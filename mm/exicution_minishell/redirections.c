/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:02:07 by maskour           #+#    #+#             */
/*   Updated: 2025/07/18 03:22:27 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_and_open_input_file(t_file *file, int *last_in_fd)
{
	int	fd;

	if (*last_in_fd != -1)
		close(*last_in_fd);
	if (file->type == TOKEN_REDIRECT_IN || file->type == TOKEN_HEREDOC)
	{
		fd = open_file(file->name, 0);
		if (fd == -1)
		{
			perror("minishell: open input");
			return (1);
		}
		*last_in_fd = fd;
	}
	return (0);
}

static int	process_input_files(t_cmd *cmd, int last_heredoc_index,
				int *last_in_fd)
{
	int		i;
	t_file	*file;

	*last_in_fd = -1;
	i = -1;
	while (++i < cmd->file_count)
	{
		file = &cmd->files[i];
		if (is_ambiguous_redirect(file->name))
			return (put_error_ambig(file->name));
		if (file->type == TOKEN_REDIRECT_IN || \
			(file->type == TOKEN_HEREDOC && i == last_heredoc_index))
		{
			if (check_and_open_input_file(file, last_in_fd))
				return (1);
		}
	}
	return (0);
}

static int	process_output_files(t_cmd *cmd, int *last_out_fd)
{
	int		i;
	int		mode;
	int		fd;
	t_file	*file;

	*last_out_fd = -1;
	i = -1;
	while (++i < cmd->file_count)
	{
		file = &cmd->files[i];
		if (file->type == TOKEN_REDIRECT_OUT || file->type == TOKEN_APPEND)
		{
			if (is_ambiguous_redirect(file->name))
				return (put_error_ambig(file->name));
			if (*last_out_fd != -1)
				close(*last_out_fd);
			mode = mode_file_type(file);
			fd = open_file(file->name, mode);
			if (fd == -1)
				return (handle_err_file(file));
			*last_out_fd = fd;
		}
	}
	return (0);
}

static int	setup_redirections(t_cmd *cmd, int last_heredoc_index,
							int original_stdin, int original_stdout)
{
	int	last_in_fd;
	int	last_out_fd;

	last_in_fd = -1;
	last_out_fd = -1;
	if (process_input_files(cmd, last_heredoc_index, &last_in_fd))
	{
		cleanup_stdio(original_stdin, original_stdout);
		return (2);
	}
	if (process_output_files(cmd, &last_out_fd))
	{
		if (last_in_fd != -1)
			close(last_in_fd);
		cleanup_stdio(original_stdin, original_stdout);
		return (2);
	}
	if (apply_input_redirection(last_in_fd, original_stdin, original_stdout))
		return (1);
	if (apply_output_redirection(last_out_fd, original_stdin, original_stdout))
		return (1);
	return (0);
}

int	redirections(t_cmd *cmd, int last_heredoc_index)
{
	int	original_stdin;
	int	original_stdout;
	int	result;

	if (!cmd || cmd->file_count <= 0)
		return (0);
	if (dup_original_fds(&original_stdin, &original_stdout))
		return (1);
	result = setup_redirections(cmd, last_heredoc_index,
			original_stdin, original_stdout);
	close(original_stdin);
	close(original_stdout);
	return (result);
}
