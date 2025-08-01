/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_v2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:15:00 by ahari             #+#    #+#             */
/*   Updated: 2025/07/27 21:52:20 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_token(t_token **head, t_token *new_token, char *str)
{
	t_token	*current;

	if (!head || !new_token || !str)
		return ;
	if (*head == NULL)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current)
	{
		if (ft_strcmp(current->value, str) == 0)
		{
			new_token->next = current->next;
			current->next = new_token;
			return ;
		}
		current = current->next;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

static int	create_and_insert_tokens(char **split, t_token **head,
	t_token *insert_after)
{
	t_token	*new_tokens;
	char	*tmp;
	int		i;

	i = 1;
	while (split[i])
	{
		tmp = ft_strjoin(split[i], "\1");
		if (!tmp)
			return (0);
		new_tokens = new_token(tmp, TOKEN_WORD);
		free(tmp);
		if (!new_tokens)
			return (0);
		append_token(head, new_tokens, insert_after->value);
		insert_after = new_tokens;
		i++;
	}
	return (1);
}

int	split_and_process_tokens(t_token *current, t_token **head, char *new_val)
{
	char	**split;

	split = ft_split(new_val);
	if (!split)
		return (free(new_val), 0);
	free(current->value);
	current->value = ft_strdup(split[0]);
	if (!current->value)
		return (free_array(split), free(new_val), 0);
	if (!create_and_insert_tokens(split, head, current))
		return (free_array(split), free(new_val), 0);
	return (free_array(split), free(new_val), 1);
}

int	process_token(t_token *current, t_token **head, t_env_list *env_list)
{
	char	*new_val;
	int		is_export_var;

	is_export_var = is_export_assignment(*head, current);
	if (current->type != TOKEN_WORD)
		return (1);
	new_val = process_quoted_value(current->value, *head, env_list);
	if (!new_val)
		return (0);
	if (is_export_var == 1 || ft_strspaces(new_val) || \
		(current->was_quoted == 1 && ft_strchr(new_val, '=') == NULL))
	{
		free(current->value);
		current->value = ft_strdup(new_val);
		if (!current->value)
			return (free(new_val),
				print_error(*head, NULL, NULL), 0);
		free(new_val);
	}
	else
	{
		if (split_and_process_tokens(current, head, new_val) == 0)
			return (print_error(*head, new_val, NULL), 0);
	}
	return (1);
}
