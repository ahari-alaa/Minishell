/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_pip_util_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:42:28 by maskour           #+#    #+#             */
/*   Updated: 2025/07/19 23:01:32 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_spaces(char *s)
{
	if (!s)
		return (1);
	while (*s)
	{
		if (*s != ' ')
			return (0);
		s++;
	}
	return (1);
}

void	cleanup_heredoc_files(t_cmd **cmds, int cmd_count)
{
	int		j;
	int		k;
	t_cmd	*cmd;

	j = -1;
	while (++j < cmd_count)
	{
		cmd = cmds[j];
		k = -1;
		while (++k < cmd->file_count)
		{
			if (cmd->files[k].type == TOKEN_HEREDOC)
				unlink(cmd->files[k].name);
		}
	}
}

int	find_last_heredoc_index(t_cmd *cmd)
{
	int	i;
	int	last;

	i = -1;
	last = -1;
	while (++i < cmd->file_count)
	{
		if (cmd->files[i].type == TOKEN_HEREDOC)
			last = i;
	}
	return (last);
}

int	process_all_heredocs(t_cmd **cmds, int cmd_count, char **env,
			t_shell *shell_ctx)
{
	t_herdoc_arg	arg;
	struct termios	original_termios;

	setup_terminal_and_signals(&original_termios);
	arg.h = -1;
	while (++(arg.h) < cmd_count)
	{
		arg.f = -1;
		while (++(arg.f) < cmds[arg.h]->file_count)
		{
			if (cmds[arg.h]->files[arg.f].type == TOKEN_HEREDOC)
			{
				arg.hd_status = function_herdoc(&cmds[arg.h]->files[arg.f], env,
						shell_ctx);
				if (arg.hd_status == 130)
				{
					shell_ctx->exit_status = 1;
					cleanup_heredoc_files(cmds, cmd_count);
					restore_terminal_and_signals(&original_termios);
					return (write(STDOUT_FILENO, "\n", 1), 0);
				}
			}
		}
	}
	return (restore_terminal_and_signals(&original_termios), 1);
}

void	setup_pipes_in_child(int i, int cmd_count, int prev_pipe,
				int pipes[2])
{
	if (i > 0)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (i < cmd_count - 1)
	{
		close(pipes[0]);
		dup2(pipes[1], STDOUT_FILENO);
		close(pipes[1]);
	}
}
