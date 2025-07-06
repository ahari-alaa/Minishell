/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_v2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:15:00 by ahari             #+#    #+#             */
/*   Updated: 2025/07/06 00:28:21 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

//
static void process_export_assignment_split(char **new_val, int i, int is_export_var)
{
    char	**split;
    char    *joined;

    if (is_export_var == 2)
    	return ;
    split = split_with_quotes(new_val[i]);
    if (!split || !split[0])
    {
        if (split)
            free_array(split);
        return ;
    }
    if (is_export_var == 1)
    {
        joined = join_export_tokens(split);
        if (joined)
        {
            free(new_val[i]);
            new_val[i] = joined;
        }
        free_array(split);
    }
}


int	finalize_token_value(t_token *current, t_token **head, char *val_cmd, char **new_val)
{
	free_array(new_val);
	free(current->value);
	if (val_cmd)
		current->value = val_cmd;
	else
		current->value = ft_strdup("");
	if (!current->value)
	{
		free_tokens(*head, NULL);
		return (0);
	}
	return (1);
}

int	join_new_value(char **val_cmd, char *str, t_token **head, char **new_val)
{
	char	*tmp;

	if (!*val_cmd)
	{
		*val_cmd = ft_strdup(str);
		if (!*val_cmd)
			return (free_array(new_val), free_tokens(*head, NULL), 0);
	}
	else
	{
		tmp = ft_strjoin(*val_cmd, str);
		free(*val_cmd);
		*val_cmd = tmp;
		if (!*val_cmd)
			return (free_array(new_val), free_tokens(*head, NULL), 0);
	}
	return (1);
}

int	handle_split(t_token *cur, t_token **head,
		char **new_val, int i)
{
	char	**split;

	split = ft_split(new_val[i]);
	if (split && split[0] && split[1])
	{
		if (!handle_token_splitting(cur, head, split))
			return (free_tokens(*head, NULL), free_array(new_val),
				free_array(split), 0);
		return (free_array(split), free_array(new_val), 2);
	}
	if (split)
		free_array(split);
	return (1);
}
int	process_token_loop(t_token *cur, t_token **head,
		t_shell *ctx, t_process p)
{
	int	i;
	int	ret;

	i = 0;
	while (p.new_val[i])
	{
		if (!is_single_quoted(cur->value, p.new_val[i]))
		{
			if (p.new_val[i][0] == '\1')
            {
                char *unmarked = ft_strdup(p.new_val[i] + 1);
                if (!unmarked)
                    return 0;
                char *expanded = found_env(unmarked, p.env, ctx);
                free(unmarked);
                if (!expanded)
                    return 0;
                free(p.new_val[i]);
                p.new_val[i] = expanded;
                if (!join_new_value(&p.val_cmd, p.new_val[i], head, p.new_val))
                    return 0;
                i++;
                continue;
            }
			if (!process_env_expansion(p.new_val, i, p.env, ctx))
				return (free_tokens(*head, NULL), free_array(p.new_val),
					free(p.val_cmd), 0);
			if (!is_quoted(cur->value, p.new_val[i]))
			{
				ret = handle_split(cur, head, p.new_val, i);
				if (ret != 1)
					return (free(p.val_cmd), ret);
			}
			else if (!is_quoted(cur->value, p.new_val[i]))
				process_export_assignment_split(p.new_val, i, p.is_export);
		}
		if (!join_new_value(&p.val_cmd, p.new_val[i], head, p.new_val))
			return (0);
		i++;
	}
	return (finalize_token_value(cur, head, p.val_cmd, p.new_val));
}
char *ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
    char    *result;
    size_t  len1;
    size_t  len2;
    size_t  len3;
    size_t  total_len;

    if (!s1 || !s2 || !s3)
        return (NULL);
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    len3 = ft_strlen(s3);
    total_len = len1 + len2 + len3 + 1;
    result = (char *)malloc(total_len * sizeof(char));
    if (!result)
        return (NULL);
    ft_strlcpy(result, s1, len1 + 1);
    ft_strlcpy(result + len1, s2, len2 + 1);
    ft_strlcpy(result + len1 + len2, s3, len3 + 1);
    return (result);
}
void export_one_case(char *value, t_token *head)
{
    char *equal_sign;
    char *var_part;
    char *val_part;
    char *new_value;
    int needs_quotes = 0;

    if (!value || !head)
        return;
    equal_sign = ft_strchr(value, '=');
    if (!equal_sign)
        return; 
    var_part = ft_strndup(value, equal_sign - value);
    val_part = ft_strdup(equal_sign + 1);
    if (!var_part || !val_part)
    {
        free(var_part);
        free(val_part);
        return;
	}
    int has_empty_quotes = (var_part[0] == '\'' && var_part[1] == '\'');
    if (ft_strchr(val_part, '$') && 
        !is_quoted(val_part, val_part) && 
        !has_empty_quotes)
    {
        needs_quotes = 1;
    }
    if (needs_quotes)
    {
        new_value = ft_strjoin3(var_part, "=\"", val_part);
        if (new_value)
        {
            new_value = ft_strjoin(new_value, "\"");
            free(value);
            head->value = new_value;
            head->was_quoted = 1;
        }
    }
    else
    {
        new_value = ft_strjoin3(var_part, "=", val_part);
        if (new_value)
        {
            free(value);
            head->value = new_value;
        }
    }
    free(var_part);
    free(val_part);
}

int process_token(t_token *current, t_token **head,
        t_shell *shell_ctx, char **env_table)
{
    char    **new_val;
    char    *val_cmd;
    int     is_export_var;

    if (current->type != TOKEN_WORD)
        return (1);
    
    is_export_var = is_export_assignment(*head, current);
    if (is_export_assignment(*head, current))
    {
        export_one_case(current->value, current);
		printf("exported: %s\n", current->value);
    }
    new_val = process_quoted_value(current->value, *head, shell_ctx);
    if (!new_val)
        return (0);
    val_cmd = NULL;
    return (process_token_loop(current, head, shell_ctx,
            (t_process){new_val, val_cmd, env_table, is_export_var}));
}
