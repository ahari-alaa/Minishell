/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:02:07 by maskour           #+#    #+#             */
/*   Updated: 2025/07/22 18:21:18 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int backup_stdio(int *original_stdin, int *original_stdout)
{
	*original_stdin = dup(STDIN_FILENO);
	*original_stdout = dup(STDOUT_FILENO);
	if (*original_stdin == -1 || *original_stdout == -1)
	{
		perror("minishell: dup");
		return 1;
	}
	return (0);
}


static int handle_input_redirect(t_file *file, int i, int last_heredoc_index, 
						 int *last_in_fd, int or_stdin, int or_stdout)
{
	if (file->type == TOKEN_REDIRECT_IN)
	{
		if (*last_in_fd != -1) close(*last_in_fd);
		*last_in_fd = open_file(file->name, 0);
		if (*last_in_fd == -1)
		{
			perror("minishell: open_file input");
			cleanup_stdio(or_stdin, or_stdout);
			return (1);
		}
	}
	else if (file->type == TOKEN_HEREDOC && i == last_heredoc_index) {
		if (*last_in_fd != -1)
			close(*last_in_fd);
		*last_in_fd = open(file->name, O_RDONLY);
		if (*last_in_fd == -1)
		{
			perror("minishell: open heredoc file");
			cleanup_stdio(or_stdin, or_stdout);
			return (1);
		}
	}
	return (0);
}

static int handle_output_redirect(t_file *file, int *last_out_fd, 
						  int original_stdin, int original_stdout)
{
	if (file->type == TOKEN_REDIRECT_OUT)
	{
		if (*last_out_fd != -1) close(*last_out_fd);
		*last_out_fd = open_file(file->name, 1);
		if (*last_out_fd == -1) {
			perror("minishell: open_file output");
			cleanup_stdio(original_stdin, original_stdout);
			return (1);
		}
	}
	else if (file->type == TOKEN_APPEND)
	{
		if (*last_out_fd != -1)
			close(*last_out_fd);
		*last_out_fd = open_file(file->name, 2);
		if (*last_out_fd == -1) {
			perror("minishell: open_file append");
			cleanup_stdio(original_stdin, original_stdout);
			return (1);
		}
	}
	return (0);
}

static int apply_final_redirections(int last_in_fd, int last_out_fd, 
						   int original_stdin, int original_stdout)
{
	if (last_in_fd != -1)
	{
		if (dup2(last_in_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 input");
			close(last_in_fd);
			cleanup_stdio(original_stdin, original_stdout);
			return (1);
		}
		close(last_in_fd);
	}
	if (last_out_fd != -1)
	{
		if (dup2(last_out_fd, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 output");
			close(last_out_fd);
			cleanup_stdio(original_stdin, original_stdout);
			return (1);
		}
		close(last_out_fd);
	}
	close(original_stdin);
	close(original_stdout);
	return (0);
}

int redirections(t_cmd *cmd, int last_heredoc_index)
{
	int or_stdin;
	int or_stdout;
	int lastinfd;
	int lastoutfd;
	int i;
	t_file *file;

	if (!cmd || cmd->file_count <= 0)
		return 0;
	if (backup_stdio(&or_stdin, &or_stdout))
		return 1;
	lastinfd = -1;
	lastoutfd = -1;
	i = -1;
	while (++i < cmd->file_count)
	{
		file = &cmd->files[i];
		handle_ambiguous_redirect(file, or_stdin, or_stdout);
		if (handle_input_redirect(file, i, last_heredoc_index, &lastinfd, 
								 or_stdin, or_stdout))
			return (1);	
		if (handle_output_redirect(file, &lastoutfd, or_stdin, or_stdout))
			return (1);
	}
	return (apply_final_redirections(lastinfd, lastoutfd, or_stdin, or_stdout));
}
