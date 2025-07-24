/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:18:09 by ahari             #+#    #+#             */
/*   Updated: 2025/07/24 11:09:37 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_here_docs(t_cmd *commands, t_shell *shell_ctx,
	t_token *tokens, char **env_table)
{
	if (count_herdoc(commands) > 16)
	{
		write(2, "minishell: too many here-documents\n", 36);
		shell_ctx->exit_status = 2;
		free_tokens(tokens, NULL);
		free_cmd_list(commands);
		free_array(env_table);
		return (1);
	}
	return (0);
}

int	handle_input(char *input, char **env_table,
	t_token **tokens, t_shell *shell_ctx)
{
	*tokens = check_quoted(input, shell_ctx, env_table);
	free(input);
	if (!(*tokens))
	{
		free(NULL);
		free_array(env_table);
		return (1);
	}
	return (0);
}

int	handle_commands(t_token *tokens, t_shell *shell_ctx,
	char **env_table, t_cmd **commands)
{
	*commands = parse_commands(tokens, shell_ctx);
	if (!(*commands))
	{
		free_tokens(tokens, NULL);
		free_array(env_table);
		return (1);
	}
	return (0);
}

int	check_pipe_syntax(t_token *head, t_shell *shell_ctx)
{
	t_token	*current;

	if (strcmp(head->value, "|") == 0)
	{
		printf("syntax error near unexpected token `|'\n");
		shell_ctx->exit_status = 258;
		return (0);
	}
	current = head;
	while (current && current->next)
	{
		if (strcmp(current->value, "|") == 0 && \
			strcmp(current->next->value, "|") == 0)
		{
			printf("syntax error near unexpected token `|'\n");
			shell_ctx->exit_status = 258;
			return (0);
		}
		current = current->next;
	}
	return (1);
}
