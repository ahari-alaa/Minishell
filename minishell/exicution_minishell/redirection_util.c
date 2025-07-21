/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:31:55 by maskour           #+#    #+#             */
/*   Updated: 2025/07/21 18:06:56 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_file(char *file, int mode)
{
	int	fd;

	fd = 0;
	if (mode == 0)
		fd = open(file, O_RDONLY);
	else if (mode == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (mode == 2)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	cleanup_stdio(int original_stdin, int original_stdout)
{
	int	ret;

	ret = 0;
	if (dup2(original_stdin, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 stdin restore failed");
		ret = 1;
	}
	if (dup2(original_stdout, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 stdout restore failed");
		ret = 1;
	}
	close(original_stdin);
	close(original_stdout);
	return (ret);
}

int	is_ambiguous_redirect(const char *filename)
{
	if (!filename)
		return (1);
	if (filename[0] == '\0')
		return (1);
	if (ft_strcmp(filename, "\2") == 0)
		return (9);
	return (0);
}

int handle_ambiguous_redirect(t_file *file, int original_stdin, int original_stdout)
{
    if (is_ambiguous_redirect(file->name))
    {
        ft_putstr_fd_up("minishell: ", 2);
        ft_putstr_fd_up(file->name, 2); 
        ft_putstr_fd(": ambiguous redirect\n", 2, 0);
        cleanup_stdio(original_stdin, original_stdout);
        exit (1); 
    }
    return 0;
}
