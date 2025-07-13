/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:04:25 by ahari             #+#    #+#             */
/*   Updated: 2025/07/13 17:08:27 by ahari            ###   ########.fr       */
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
	t_token *current;

	if (!head || head == target)
		return NULL;
	current = head;
	while (current->next && current->next != target)
		current = current->next;
	if (current->next == target)
		return current;
	else
		return  NULL;
}

t_token *check_quoted(char *str, t_shell *shell_ctx, char **env_tab)
{
	t_token *head;
	t_token *current;
	t_token *prev;
	int     process_result;
	t_env_list	*env_list;

	env_list = malloc(sizeof(t_env_list));
	if (!env_list)
		return NULL;
	env_list->env_table = env_tab;
	env_list->exit_status = shell_ctx;
	head = string_tokens(str, shell_ctx);
	if (!head)
	{
		free(env_list);
		return NULL;
	}
	current = head;
	while (current)
	{
		prev = find_previous_token(head, current);
		if (prev && prev->type == TOKEN_HEREDOC)
		{
			char *new_val = herdoc_parsing(current->value);
			if (!new_val)
			{
				free(env_list);
				return (free_tokens(head, str), NULL);
			}
			free(current->value);
			current->value = new_val;
		}
		else
		{
			process_result = process_token(current, &head, env_list);
			if (process_result == 0)
			{
				free(env_list);
				return (free_tokens(head, str), NULL);
			}
		}
		current = current->next;
	}
	free(env_list);
	return head;
}

// char *process_quoted_value(char *val, t_token *head, t_env_list *env)
// {
//     char    *result;
//     char    *tmp = NULL;
//     int     i;
//     int     start;
//     char    quote;
//     char    *expanded;
//     size_t  result_capacity;
//     size_t  result_len;

//     i = 0;
//     if (!val)
//         return (print_error(head, NULL, NULL), NULL);

//     // Start with initial allocation
//     result_capacity = ft_strlen(val) + 1;
//     result = malloc(result_capacity);
//     if (!result)
//         return (print_error(head, NULL, NULL), NULL);
//     result[0] = '\0';

//     while (val[i])
//     {
//         if (val[i] == '\'' || val[i] == '\"')
//         {
//             quote = val[i++];
//             start = i;
//             while (val[i] && val[i] != quote)
//                 i++;
//             tmp = ft_substr(val, start, i - start);
//             if (!tmp)
//             {
//                 free(result);
//                 return (NULL);
//             }
//             if (quote == '\"')
//             {
//                 expanded = found_env(tmp, env->env_table, env->exit_status);
//                 free(tmp);
//                 tmp = expanded;
//             }

//             // Ensure enough capacity before strcat
//             result_len = ft_strlen(result) + ft_strlen(tmp) + 1;
//             if (result_len > result_capacity)
// 			{
//                 result_capacity = result_len * 2;
//                 char *new_result = realloc(result, result_capacity);
//                 if (!new_result)
//                     return (free(tmp), free(result), NULL);
//                 result = new_result;
//             }
//             ft_strcat(result, tmp);
//             free(tmp);
//             if (val[i] == quote)
//                 i++;
//         }
//         else
//         {
//             start = i;
//             while (val[i] && val[i] != '\'' && val[i] != '\"')
//                 i++;
//             tmp = ft_substr(val, start, i - start);
//             if (!tmp)
//                 return (free(result), NULL);
//             expanded = found_env(tmp, env->env_table, env->exit_status);
//             free(tmp);
//             tmp = expanded;
//             if (ft_strcmp(tmp, "$") == 0)
//             {
//                 free(tmp);
//                 tmp = ft_strdup("\1");
//             }
//             result_len = ft_strlen(result) + ft_strlen(tmp) + 1;
//             if (result_len > result_capacity)
// 			{
//                 result_capacity = result_len * 2;
//                 char *new_result = realloc(result, result_capacity);
//                 if (!new_result)
//                     return (free(tmp), free(result), NULL);
//                 result = new_result;
//             }
//             ft_strcat(result, tmp);
//             free(tmp);
//         }
//     }
//     if (ft_strcmp(result, "\1") == 0)
//     {
//         free(result);
//         result = ft_strdup("$");
//     }
//     return (result);
// }

// Function to parse heredoc values by removing quotes and keeping dollar signs intact
// char *herdoc_parsing(char *val)
// {
// 	char	*result;
// 	char	*tmp;
// 	int		i;
// 	int		start;
// 	char	quote;

// 	i = 0;
// 	if (!val)
// 		return (NULL);

// 	result = malloc(sizeof(char) * (ft_strlen(val) + 1));
// 	if (!result)
// 		return (NULL);

// 	result[0] = '\0';

// 	while (val[i])
// 	{
// 		if (val[i] == '\'' || val[i] == '\"')
// 		{
// 			quote = val[i++];
// 			start = i;
// 			while (val[i] && val[i] != quote)
// 				i++;
// 			tmp = ft_strndup(val + start, i - start);
// 			if (!tmp)
// 			{
// 				free(result);
// 				return (NULL);
// 			}
// 			// Append the content inside quotes to result
// 			if (ft_strcmp(result, "$") == 0)
// 				result = ft_strdup("");
// 			ft_strcat(result, tmp);
// 			free(tmp);
// 			if (val[i] == quote)
// 				i++;
// 		}
// 		else
// 		{
// 			start = i;
// 			while (val[i] && val[i] != '\'' && val[i] != '\"')
// 				i++;
// 			tmp = ft_strndup(val + start, i - start);
// 			if (!tmp)
// 			{
// 				free(result);
// 				return (NULL);
// 			}
// 			// Append the content outside quotes to result
// 			ft_strcat(result, tmp);
// 			if(ft_strcmp(tmp, "$") == 0)
// 				tmp = ft_strdup("\1");
// 			free(tmp);
// 		}
// 	}
//     if(ft_strcmp(result, "\1") == 0)
// 		result = ft_strdup("$");
// 	return (result);
// }

// t_token *check_quoted(char *str, t_shell *shell_ctx, char **env_tab)
// {
// 	t_token *head;
// 	t_token *current;
// 	t_token *prev;
// 	int     process_result;
// 	t_env_list	*env_list;

// 	env_list = malloc(sizeof(t_env_list));
// 	if (!env_list)
// 		{return NULL;}
// 	env_list->env_table = env_tab;
// 	env_list->exit_status = shell_ctx;
// 	head = string_tokens(str, shell_ctx);
// 	if (!head)
// 		return (NULL);
// 	current = head;
// 	while (current)
// 	{
// 		prev = find_previous_token(head, current);
// 		if (prev && prev->type == TOKEN_HEREDOC)
// 		{
// 			current->value = herdoc_parsing(current->value);
// 			if (!current->value)
// 			      return (free_tokens(head, str), NULL);
// 		}
// 		else
// 		{
// 			process_result = process_token(current, &head, env_list);
// 			if (process_result == 0)
//                return (free_tokens(head, str), NULL);
// 		}
// 		current = current->next;
// 	}
// 	return head;
// }
