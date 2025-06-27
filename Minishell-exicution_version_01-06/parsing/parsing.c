/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:04:25 by ahari             #+#    #+#             */
/*   Updated: 2025/06/27 21:26:22 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

t_token *find_previous_token(t_token *head, t_token *target)
{
    if (!head || head == target)
        return NULL;
    t_token *current = head;
    while (current->next && current->next != target)
        current = current->next;
    if (current->next == target)
        return current;
    else
        return  NULL;
}

static int is_export_assignment(t_token *head, t_token *current)
{
    t_token *temp = head;
    
    while (temp && temp != current)
    {
        if (temp->type == TOKEN_PIPE || temp->type == TOKEN_SEMICOLON)
        {
            if (temp->next)
                temp = temp->next;
            break;
        }
        temp = temp->next;
    }
    temp = head;
    t_token *cmd_token = NULL;
    while (temp && temp != current)
    {
        if (temp->type == TOKEN_PIPE || temp->type == TOKEN_SEMICOLON)
            cmd_token = NULL;
        else if (temp->type == TOKEN_WORD)
        {
            if (!cmd_token)
                cmd_token = temp;
        }
        temp = temp->next;
    }
    if (cmd_token && ft_strcmp(cmd_token->value, "export") == 0)
    {
        int i = 0;
        char *str = current->value;
        while (str[i])
        {
            if (str[i] == '\'' || str[i] == '"')
                return (2);
            if (str[i] == '=')
                break;
            i++;
        }
        if (ft_strchr(current->value, '='))
            return (1);
    }
    
    return (0);
}

// Special handling for export assignments - don't split after '='
static char *join_export_tokens(char **split)
{
    char *result = NULL;
    char *temp = NULL;
    int i = 0;
    char *equals_pos = ft_strchr(split[0], '=');
    if (!equals_pos)
        return NULL;
    result = ft_strdup(split[0]);
    if (!result)
        return NULL;
    i = 1;
    while (split[i])
    {
        temp = ft_strjoin(result, " ");
        free(result);
        if (!temp)
            return NULL;
        result = temp;
        temp = ft_strjoin(result, split[i]);
        free(result);
        if (!temp)
            return NULL;
        result = temp;
        i++;
    }
    return result;
}

char **process_quoted_value(char *val, t_token *head, t_shell *shell_ctx)
{
    char    **strings = NULL;
    int     i = 0, start, count = 0;
    char    quote;
    char    *tmp;
    
    (void)head;
    if (!val)
        return (print_error(NULL, NULL, shell_ctx), NULL);
    strings = malloc(sizeof(char *) * (ft_strlen(val) + 1));
    if (!strings)
        return (print_error(NULL, NULL, shell_ctx), NULL);
    while (val[i])
    {
        if (val[i] == '\'' || val[i] == '\"')
        {
            quote = val[i];
            i++;
            start = i;
            while (val[i] && val[i] != quote)
                i++;
            tmp = ft_substr(val, start, i - start);
            if (!tmp)
            {
                free_array(strings);
                return (NULL);
            }
            if (*tmp)
                strings[count++] = tmp;
            else
                free(tmp);
            if (val[i] == quote)
                i++;
        }
        else
        {
            start = i;
            while (val[i] && val[i] != '\'' && val[i] != '\"')
                i++;
            tmp = ft_substr(val, start, i - start);
            if (!tmp)
            {
                free_array(strings);
                return (NULL);
            }
            if (*tmp)
                strings[count++] = tmp;
            else
                free(tmp);
        }
    }
    strings[count] = NULL;
    return strings;
}

static int is_single_quoted(char *original_val, char *substring)
{
    char *pos = ft_strstr(original_val, substring);
    int quote_count = 0;
    int i = 0;
    
    if (!pos)
        return 0;
    
    while (&original_val[i] < pos)
    {
        if (original_val[i] == '\'' && (i == 0 || original_val[i - 1] != '\\'))
            quote_count++;
        i++;
    }
    return (quote_count % 2 == 1);
}

static int is_quoted(char *original_val, char *substring)
{
    char *pos = ft_strstr(original_val, substring);
    int i = 0;
    char current_quote = 0;
    
    if (!pos)
        return 0;
    while (&original_val[i] < pos)
    {
        if ((original_val[i] == '\'' || original_val[i] == '"') && 
            (i == 0 || original_val[i - 1] != '\\'))
        {
            if (!current_quote)
                current_quote = original_val[i];
            else if (original_val[i] == current_quote)
                current_quote = 0;
        }
        i++;
    }
    return (current_quote != 0);
}

char **split_with_quotes(char *str)
{
    int i = 0, start = 0, count = 0;
    char **result = NULL;
    int in_quotes = 0;
    char quote_char = 0;
    
    if (!str || !*str)
        return NULL;
    while (str[i])
    {
        while (ft_isspace(str[i]))
            i++;   
        if (!str[i])
            break;
        count++;
        start = i;
        while (str[i])
        {
            if (str[i] == '\'' || str[i] == '"')
            {
                if (!in_quotes)
                {
                    in_quotes = 1;
                    quote_char = str[i];
                }
                else if (str[i] == quote_char)
                {
                    in_quotes = 0;
                    quote_char = 0;
                }
                i++;
            }
            else if (!in_quotes && ft_isspace(str[i]))
                break;
            else
                i++;
        }
    }
    if (count == 0)
        return NULL;
    result = malloc(sizeof(char *) * (count + 1));
    if (!result)
        return NULL;
    i = 0;
    count = 0;
    in_quotes = 0;
    quote_char = 0;
    while (str[i])
    {
        while (ft_isspace(str[i]))
            i++;       
        if (!str[i])
            break;
        start = i;
        while (str[i])
        {
            if (str[i] == '\'' || str[i] == '"')
            {
                if (!in_quotes)
                {
                    in_quotes = 1;
                    quote_char = str[i];
                }
                else if (str[i] == quote_char)
                {
                    in_quotes = 0;
                    quote_char = 0;
                }
                i++;
            }
            else if (!in_quotes && ft_isspace(str[i]))
                break;
            else
                i++;
        }
        result[count] = ft_strndup(str + start, i - start);
        if (!result[count])
        {
            free_array(result);
            return NULL;
        }
        count++;
    }
    result[count] = NULL;
    return result;
}

static int handle_token_splitting(t_token *current, t_token **head, char **split)
{
    t_token *next_token = current->next;
    t_token *prev_token = find_previous_token(*head, current);
    t_token *first_new = NULL;
    t_token *last_new = NULL;
    int j = 0;

    while (split[j])
    {
        t_token *new_tokens = new_token(ft_strdup(split[j]), TOKEN_WORD);
        if (!new_tokens)
        {
            if (first_new)
                free_tokens(first_new, NULL);
            return (0);
        }
        if (!first_new)
            first_new = new_tokens;
        
        if (last_new)
            last_new->next = new_tokens;
        last_new = new_tokens;
        j++;
    }
    if (prev_token)
        prev_token->next = first_new;
    else
        *head = first_new;
    last_new->next = next_token;
    free(current->value);
    free(current);
    
    return (1);
}

static int process_env_expansion(char **new_val, int i, char **env_table, t_shell *shell_ctx)
{
    char *expanded = found_env(new_val[i], env_table, shell_ctx);
    if (!expanded)
        return (0);
    free(new_val[i]);
    new_val[i] = expanded;
    return (1);
}

static int process_token(t_token *current, t_token **head, t_shell *shell_ctx, char **env_table)
{     
    char **new_val = NULL;
    char *val_cmd = NULL;
    int i;
    int is_export_var = 0;
    
    if (current->type != TOKEN_WORD)         
        return (1);
    is_export_var = is_export_assignment(*head, current);
    new_val = process_quoted_value(current->value, *head, shell_ctx);
    if (!new_val)         
        return (0);
    i = 0;     
    while(new_val[i])     
    {         
        if (!is_single_quoted(current->value, new_val[i]))         
        {             
            if (!process_env_expansion(new_val, i, env_table, shell_ctx))
            {
                free_array(new_val);
                free_tokens(*head, NULL);
                return (0);             
            }
            if (!is_quoted(current->value, new_val[i]) && !is_export_var)
            {
                char **split = split_with_quotes(new_val[i]);
                if (split && split[0] && split[1])
                {
                    if (!handle_token_splitting(current, head, split))
                    {
                        free_array(split);
                        free_array(new_val);
                        free_tokens(*head, NULL);
                        return (0);
                    }
                    free_array(split);
                    free_array(new_val);
                    return (2);
                }
                if (split)
                    free_array(split);
            }
            else if (is_export_var && !is_quoted(current->value, new_val[i]))
            {
                if (is_export_var == 1)
                {
                    char **split = split_with_quotes(new_val[i]);
                    if (split && split[0] && split[1])
                    {
                        char *joined = join_export_tokens(split);
                        if (joined)
                        {
                            free(new_val[i]);
                            new_val[i] = joined;
                        }
                        free_array(split);
                    }
                }
                else if (is_export_var == 2)
                {
                    char **split = split_with_quotes(new_val[i]);
                    if (split && split[0] && split[1])
                    {
                        char *joined = split[0];
                        if (joined)
                            new_val[i] = joined;
                    }
                }
            }
        }
        
        if (new_val[i])
        {
            if (i == 0)
                val_cmd = ft_strdup(new_val[i]);   
            else
            {
                char *tmp = ft_strjoin(val_cmd, new_val[i]);
                free(val_cmd);             
                val_cmd = tmp;             
                if (!val_cmd)
                {                 
                    free_array(new_val);                 
                    free_tokens(*head, NULL);                 
                    return (0);             
                }         
            }         
        }
        i++;     
    }
    
    free_array(new_val);
    if (val_cmd)
    {
        free(current->value);     
        current->value = val_cmd;     
    }
    else
    {
        free(current->value);
        current->value = ft_strdup("");
    }
    if (!current->value)     
    {         
        free_tokens(*head, NULL);         
        return (0);     
    }     
    return (1); 
}

t_token *check_quoted(char *str, t_shell *shell_ctx, char **env_table)
{
    t_token *head;
    t_token *current;
    t_token *next;
    int process_result;

    head = string_tokens(str, shell_ctx);
    if (!head)
        return (NULL);
    current = head;
    while (current)
    {
        next = current->next;
        process_result = process_token(current, &head, shell_ctx, env_table);
        if (process_result == 0)
            return NULL;
        else if (process_result == 2)
        {
            current = head;
            continue;
        }
        current = next;
    }
    return head;
}
