/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_util_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:35:40 by maskour           #+#    #+#             */
/*   Updated: 2025/07/17 17:42:24 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	apply_input_redirection(int last_in_fd, int orig_stdin,
				int orig_stdout)
{
	if (last_in_fd == -1)
		return (0);
	if (dup2(last_in_fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 input");
		close(last_in_fd);
		cleanup_stdio(orig_stdin, orig_stdout);
		return (1);
	}
	close(last_in_fd);
	return (0);
}

int	apply_output_redirection(int last_out_fd,
					int orig_stdin, int orig_stdout)
{
	if (last_out_fd == -1)
		return (0);
	if (dup2(last_out_fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 output");
		close(last_out_fd);
		cleanup_stdio(orig_stdin, orig_stdout);
		return (1);
	}
	close(last_out_fd);
	return (0);
}
