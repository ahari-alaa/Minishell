/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_multiple_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:47:51 by maskour           #+#    #+#             */
/*   Updated: 2025/07/13 13:47:29 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	wait_for_children(pid_t last_pid, t_shell *shell_ctx)
{
	int		wstatus;
	pid_t	wpid;
	int		sig;
	int		saw_sigint;

	wstatus = 0;
	saw_sigint = 0;
	while ((wpid = wait(&wstatus)) > 0)
	{
		if (WIFSIGNALED(wstatus))
		{
			sig = WTERMSIG(wstatus);
			if (sig == SIGINT)
				saw_sigint = 1;
			if (sig == SIGQUIT)
				write(1, "Quit: 3\n", 8);
		}
		if (wpid == last_pid)
			update_exit_status(wstatus, shell_ctx);
	}
	if (saw_sigint)
		write(1, "\n", 1);
}

static int	handle_pipeline_error(t_cmd **cmds, int cmd_count,
			const char *context, t_shell *shell_ctx)
{
	perror(context);
	cleanup_heredoc_files(cmds, cmd_count);
	shell_ctx->exit_status = 1;
	return (1);
}

static int	handle_pipeline_iteration(t_cmd **cmds,
	char **env, t_env *env_list, t_exec_ctx *ctx)
{
	pid_t	pid;

	if (ctx->i < ctx->cmd_count - 1 && pipe(ctx->pipes) == -1)
		return (handle_pipeline_error(cmds, ctx->cmd_count,
			"minishell: pipe", ctx->shell_ctx));
	pid = fork();
	if (pid == 0)
		execute_child(cmds, env, env_list, ctx);
	else if (pid > 0)
	{
		handle_parent_process(ctx->i, ctx->prev_pipe, ctx->pipes, ctx->cmd_count);
		*(ctx->last_pid) = pid;
	}
	else
		return (handle_pipeline_error(cmds, ctx->cmd_count, "minishell: fork", ctx->shell_ctx));
	return (0);
}

static void	fork_and_execute_pipeline(t_cmd **cmds, char **env,
	t_env *env_list, t_exec_ctx *ctx)
{
	ctx->i = -1;
	while (++(ctx->i) < ctx->cmd_count && cmds[ctx->i])
	{
		ctx->cmd_index = ctx->i;
		if (handle_pipeline_iteration(cmds, env, env_list, ctx))
			break ;
	}
}

void	execute_pipeline(t_cmd **cmds, char **env,
	t_env *env_list, t_exec_ctx *exec_ctx)
{
	int		prev_pipe;
	pid_t	last_pid;

	if (!cmds || exec_ctx->cmd_count <= 0)
		return ;
	if (!process_all_heredocs(cmds, exec_ctx->cmd_count, env, exec_ctx->shell_ctx))
		return ;
	exec_ctx->prev_pipe = &prev_pipe;
	exec_ctx->last_pid = &last_pid;
	ignore_sigint();
	fork_and_execute_pipeline(cmds, env, env_list, exec_ctx);
	if (prev_pipe != -1)
		close(prev_pipe);
	wait_for_children(last_pid, exec_ctx->shell_ctx);
	cleanup_heredoc_files(cmds, exec_ctx->cmd_count);
	restore_sigint();
}
