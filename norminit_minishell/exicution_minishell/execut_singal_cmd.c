/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_singal_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:42:06 by maskour           #+#    #+#             */
/*   Updated: 2025/07/13 12:47:38 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// static int	process_heredocs(t_cmd *cmd, char **env, t_shell *shell, 
// 			int *last_heredoc_index)
// {
// 	int	i;
// 	int	hd_status;

// 	i = -1;
// 	while (++i < cmd->file_count)
// 	{
// 		if (cmd->files[i].type == TOKEN_HEREDOC)
// 		{
// 			hd_status = function_herdoc(&cmd->files[i], env, shell);
// 			if (hd_status == 130)
// 			{
// 				cleanup_herdoc(cmd);
// 				exit(hd_status);
// 			}
// 			*last_heredoc_index = i;
// 		}
// 	}
// 	return (0);
// }

// static void	execute_command(char *cmd_path, t_cmd *cmd, char **env)
// {
// 	if (execve(cmd_path, cmd->cmd, env) == -1)
// 	{
// 		if (cmd->cmd[0][0] == '\0')
// 		{
// 			ft_putstr_fd_up("minishell:", 2);
// 			ft_putstr_fd_up(" command not found\n", 2);
// 			exit(127);
// 		}
// 		if (ft_strstr(cmd->cmd[0], "./") != NULL)
// 		{
// 			ft_putstr_fd_up("minishell:", 2);
// 			ft_putstr_fd_up(cmd->cmd[0], 2);
// 			ft_putstr_fd_up(":Is a directory\n", 2);
// 		}
// 		else
// 			handle_cmd_errors(cmd_path);
// 		free(cmd_path);
// 		exit(126);
// 	}
// }

// static int	cmd_process(t_cmd *cmd, t_env **env_list,
// 				char **env, t_shell *shell)
// {
// 	char	*cmd_path;
// 	int		last_heredoc_index;

// 	last_heredoc_index = -1;
// 	process_heredocs(cmd, env, shell, &last_heredoc_index);
// 	if (redirections(cmd, last_heredoc_index) == 2)
// 	{
// 		cleanup_herdoc(cmd);
// 		exit(1);
// 	}
// 	handle_empty_cmd(cmd);
// 	if (is_builtin(cmd->cmd[0]))
// 	{
// 		*env_list = execut_bultin(&cmd, *env_list, shell, 1);
// 		// IMPORTANT: Cleanup heredocs before exiting builtin
// 		cleanup_herdoc(cmd);
// 		exit(shell->exit_status);  // Exit with builtin's status
// 	}
// 	cmd_path = resolve_cmd_path(cmd, env);
// 	execute_command(cmd_path, cmd, env);
// 	// This line should never be reached due to execve or exit
// 	cleanup_herdoc(cmd);  // Safety cleanup
// 	exit(0);
// }

// static void	handle_child_exit_status(int status, t_shell *shell_ctx)
// {
// 	int	code;

// 	if (WIFEXITED(status))
// 	{
// 		code = WEXITSTATUS(status);
// 		if (code == 130)
// 		{
// 			shell_ctx->exit_status = 1;
// 			write(1, "\n", 1);
// 		}
// 		else
// 			shell_ctx->exit_status = code;
// 	}
// 	else if (WIFSIGNALED(status))
// 	{
// 		shell_ctx->exit_status = 128 + WTERMSIG(status);
// 		if (WTERMSIG(status) == SIGINT)
// 			write(1, "\n", 1);
// 		else if (WTERMSIG(status) == SIGQUIT)
// 			write(1, "Quit\n", 5);
// 	}
// 	else
// 		shell_ctx->exit_status = 0;
// }

// void	execute_single_command(t_cmd **cmd, t_env **env_list,
// 				char **envp, t_shell *shell_ctx)
// {
// 	pid_t			id;
// 	int				status;
// 	struct termios	original_termios;

// 	setup_terminal_and_signals(&original_termios);
// 	id = fork();
// 	if (id == 0)
// 	{
// 		// Child process
// 		signal(SIGINT, SIG_DFL);
// 		signal(SIGQUIT, SIG_DFL);
// 		cmd_process(*cmd, env_list, envp, shell_ctx);
// 		exit(0);  // Should never reach here
// 	}
// 	else if (id > 0)
// 	{
// 		// Parent process
// 		waitpid(id, &status, 0);
// 		restore_terminal_and_signals(&original_termios);
// 		handle_child_exit_status(status, shell_ctx);
		
// 		// CRITICAL: Parent should also cleanup heredocs
// 		// after child finishes to ensure temp files are removed
// 		cleanup_herdoc(*cmd);
// 	}
// 	else
// 	{
// 		perror("fork failed");
// 		restore_terminal_and_signals(&original_termios);
// 		// Cleanup on fork failure
// 		cleanup_herdoc(*cmd);
// 	}
// }

// void	cleanup_herdoc(t_cmd *cmd)
// {
// 	int	k;

// 	if (!cmd)
// 		return;
// 	k = -1;
// 	while (++k < cmd->file_count)
// 	{
// 		if (cmd->files[k].type == TOKEN_HEREDOC)
// 		{
// 			// Close file descriptor if it's open
// 			if (cmd->files[k].fd > 0)
// 			{
// 				close(cmd->files[k].fd);
// 				cmd->files[k].fd = -1;
// 			}
// 			// Remove temporary file
// 			unlink(cmd->files[k].name);
// 		}
// 	}
// }

// void	handle_empty_cmd(t_cmd *cmd)
// {
// 	if (!cmd || !cmd->cmd)
// 	{
// 		ft_putstr_fd_up("minishell:", 2);
// 		ft_putstr_fd_up(" command not found\n", 2);
// 		exit(127);
// 	}
// 	if (!cmd->cmd[0])
// 	{
// 		handle_cmd_errors(NULL);
// 		exit(0);
// 	}
// }

// char	*resolve_cmd_path(t_cmd *cmd, char **env)
// {
// 	char	*cmd_path;

// 	cmd_path = find_path(cmd->cmd[0], env);
// 	if (!cmd_path)
// 	{
// 		if (access(cmd->cmd[0], F_OK) == 0 && ft_check_dir(cmd->cmd[0]))
// 		{
// 			erro_handle(cmd->cmd[0], ": Permission denied\n");
// 			exit(126);
// 		}
// 		else if (opendir(cmd->cmd[0]) == NULL && \
// 			cmd->cmd[0][ft_strlen(cmd->cmd[0]) - 1] == '/')
// 		{
// 			erro_handle(cmd->cmd[0], ": Not a directory\n");
// 			exit(126);
// 		}
// 		if (ft_strstr(cmd->cmd[0], "./") != NULL)
// 			erro_handle(cmd->cmd[0], ": No such file or directory\n");
// 		else
// 			erro_handle(cmd->cmd[0], ": command not found\n");
// 		exit(127);
// 	}
// 	return (cmd_path);
// }
static int	process_heredocs(t_cmd *cmd, char **env, t_shell *shell, int *last_heredoc_index)
{
	int	i;
	int	hd_status;

	i = -1;
	while (++i < cmd->file_count)
	{
		if (cmd->files[i].type == TOKEN_HEREDOC)
		{
			hd_status = function_herdoc(&cmd->files[i], env, shell);
			if (hd_status == 130)
			{
				cleanup_herdoc(cmd);
				exit(hd_status);
			}
			*last_heredoc_index = i;
		}
	}
	return (0);
}

static void	execute_command(char *cmd_path, t_cmd *cmd, char **env)
{
	if (execve(cmd_path, cmd->cmd, env) == -1)
	{
		if (cmd->cmd[0][0] == '\0')
		{
			ft_putstr_fd_up("minishell:", 2);
			ft_putstr_fd_up(" command not found\n", 2);
			exit(127);
		}
		if (ft_strstr(cmd->cmd[0], "./") != NULL)
		{
			ft_putstr_fd_up("minishell:", 2);
			ft_putstr_fd_up(cmd->cmd[0], 2);
			ft_putstr_fd_up(":Is a directory\n", 2);
		}
		else
			handle_cmd_errors(cmd_path);
		free(cmd_path);
		exit(126);
	}
}

static int	cmd_process(t_cmd *cmd, t_env **env_list,
				char **env, t_shell *shell)
{
	char	*cmd_path;
	int		last_heredoc_index;

	last_heredoc_index = -1;
	process_heredocs(cmd, env, shell, &last_heredoc_index);
	if (redirections(cmd, last_heredoc_index) == 2)
	{
		cleanup_herdoc(cmd);
		exit(1);
	}
	handle_empty_cmd(cmd);
	if (is_builtin(cmd->cmd[0]))
		*env_list = execut_bultin(&cmd,*env_list, shell, 1);
	cmd_path = resolve_cmd_path(cmd, env);
	execute_command(cmd_path, cmd, env);
	exit(0);
}

static void	handle_child_exit_status(int status, t_shell *shell_ctx)
{
	int	code;

	if (WIFEXITED(status))
	{
		code = WEXITSTATUS(status);
		if (code == 130)
		{
			shell_ctx->exit_status = 1;
			write(1, "\n", 1);
		}
		else if (code == 2)
			shell_ctx->exit_status = 0;
		else
			shell_ctx->exit_status = code;
	}
	else if (WIFSIGNALED(status))
	{
		shell_ctx->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit\n", 5);
	}
	else
		shell_ctx->exit_status = 0;
}

void	execute_single_command(t_cmd **cmd, t_env **env_list,
				char **envp, t_shell *shell_ctx)
{
	pid_t	id;
	int		status;
	struct termios	original_termios;

	setup_terminal_and_signals(&original_termios);
	id = fork();
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		cmd_process(*cmd, env_list, envp, shell_ctx);
		exit(0);
	}
	else if (id > 0)
	{
		waitpid(id, &status, 0);
		restore_terminal_and_signals(&original_termios);
		handle_child_exit_status(status, shell_ctx);
	}
	else
	{
		perror("fork failed");
		restore_terminal_and_signals(&original_termios);
	}
}
