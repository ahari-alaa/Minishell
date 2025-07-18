/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:23:33 by maskour           #+#    #+#             */
/*   Updated: 2025/07/13 14:26:57 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_heredoc_line(t_file *file, char **env,
					t_shell *shell_ctx, t_herdoc_arg *arg)
{
	char	*expanded_line;

	if (file->check_expand == 0 && ft_strcmp(arg->line, file->name) != 0)
	{
		expanded_line = found_env(arg->line, env, shell_ctx);
		if (!expanded_line)
		{
			close(arg->fd);
			unlink(file->name);
			exit(1);
		}
		write(arg->fd, expanded_line, ft_strlen(expanded_line));
		write(arg->fd, "\n", 1);
		free(expanded_line);
	}
	else
	{
		write(arg->fd, arg->line, ft_strlen(arg->line));
		write(arg->fd, "\n", 1);
	}
	return (0);
}

static void	heredoc_child_loop(t_file *file, char **env,
					t_shell *shell_ctx, int fd)
{
	t_herdoc_arg	arg;

	arg.fd = fd;
	while (1)
	{
		arg.line = readline("> ");
		if (!arg.line)
		{
			close(fd);
			exit(2);
		}
		if (!ft_strcmp(arg.line, file->name))
		{
			free(arg.line);
			break ;
		}
		process_heredoc_line(file, env, shell_ctx, &arg);
		free(arg.line);
	}
}

static void	heredoc_child_process(t_file *file, char **env,
				t_shell *shell_ctx, char *filename)
{
	int	fd;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		free(filename);
		exit(1);
	}
	heredoc_child_loop(file, env, shell_ctx, fd);
	close(fd);
	free(filename);
	exit(0);
}

static int	heredoc_handle_status(int status, t_file *file, char *filename)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(filename);
		free(filename);
		return (130);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
	{
		free(file->name);
		file->name = filename;
		return (2);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
	{
		free(file->name);
		file->name = filename;
		return (0);
	}
	unlink(filename);
	free(filename);
	return (0);
}

int	function_herdoc(t_file *file, char **env, t_shell *shell_ctx)
{
	char	*filename;
	pid_t	pid;
	int		status;

	filename = get_rundem_name(file->name);
	if (!filename)
	{
		perror("minishell: cannot create temp file");
		free(filename);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		free(filename);
		perror("minishell: fork (heredoc)");
		return (1);
	}
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
		heredoc_child_process(file, env, shell_ctx, filename);
	waitpid(pid, &status, 0);
	return (heredoc_handle_status(status, file, filename));
}
