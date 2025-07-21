/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_pip_util.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:41:13 by maskour           #+#    #+#             */
/*   Updated: 2025/07/21 18:51:04 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_cmd_not_found(char *cmd, int i)
{
	if (access(cmd, F_OK) == 0)
	{
		erro_handle(cmd, ": Permission denied\n");
		exit(126);
	}
	if (i == 0)
	{
		if (ft_strstr(cmd, "/") != NULL)
			erro_handle(cmd, ": No such file or directory\n");
		else
		{
			erro_handle(cmd, ": command not found\n");
		}
	}
	else
	{
		if (ft_strstr(cmd, "127") != NULL)
			erro_handle("0", ": command not found\n");
		else
			erro_handle(cmd, ": command not found\n");
	}
	exit(127);
}

static void	execute_external_command(char *cmd, char **argv, char **env, int i)
{
	char	*path;

	path = find_path(cmd, env);
	if (!path)
		handle_cmd_not_found(cmd, i);
	if (execve(path, argv, env) == -1)
	{
		if (ft_strstr(cmd, "./") != NULL)
		{
			ft_putstr_fd_up("minishell:", 2);
			ft_putstr_fd_up(cmd, 2);
			ft_putstr_fd_up(":Is a directory\n", 2);
		}
		else
			handle_cmd_errors(path);
	}
	handle_cmd_errors(path);
	free(path);
	exit(126);
}

void	execute_child(t_cmd **cmds,
	char **env, t_env *env_list, t_exec_ctx *ctx)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_pipes_in_child(ctx->i, ctx->cmd_count,
		*(ctx->prev_pipe), ctx->pipes);
	if (redirections(cmds[ctx->i],
			find_last_heredoc_index(cmds[ctx->i])) != 0)
		exit(1);
	if (!cmds[ctx->i]->cmd || !cmds[ctx->i]->cmd[0] || \
			is_spaces(cmds[ctx->i]->cmd[0]))
		exit(0);
	if (is_builtin(cmds[ctx->i]->cmd[0]))
	{
		env_list = execut_bultin(&cmds[ctx->i],
				env_list, ctx->shell_ctx, 0);
		free_env(env);
		if (ctx->i == 0)
			exit(ctx->shell_ctx->exit_status);
		exit(0);
	}
	execute_external_command(cmds[ctx->i]->cmd[0],
		cmds[ctx->i]->cmd, env, ctx->i);
}

void	handle_parent_process(int i, int *prev_pipe, int pipes[2],
			int cmd_count)
{
	if (i > 0 && *prev_pipe != -1)
		close(*prev_pipe);
	if (i < cmd_count - 1)
	{
		close(pipes[1]);
		*prev_pipe = pipes[0];
	}
}

void	update_exit_status(int wstatus, t_shell *shell_ctx)
{
	if (WIFEXITED(wstatus))
		shell_ctx->exit_status = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		shell_ctx->exit_status = 128 + WTERMSIG(wstatus);
	else
		shell_ctx->exit_status = 1;
}
