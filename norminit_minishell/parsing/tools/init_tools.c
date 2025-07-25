/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:44:19 by ahari             #+#    #+#             */
/*   Updated: 2025/07/14 02:05:18 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->files = NULL;
	cmd->file_count = 0;
	return (cmd);
}

t_file	*init_mfile(void)
{
	t_file	*file;

	file = malloc(sizeof(t_file));
	if (!file)
		return (NULL);
	file->name = NULL;
	file->type = TOKEN_NULL;
	return (file);
}

t_token	*new_token(char *val, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (val)
		token->value = ft_strdup(val);
	else
		token->value = NULL;
	token->type = type;
	token->was_quoted = 0;
	token->next = NULL;
	return (token);
}

int	ft_isredirect(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT || \
		type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

int	count_args(t_token *token)
{
	int		count;
	t_token	*current;

	count = 0;
	current = token;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
			count++;
		else if (ft_isredirect(current->type))
		{
			if (current->next)
				current = current->next;
		}
		current = current->next;
	}
	return (count);
}
