/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:23:28 by maskour           #+#    #+#             */
/*   Updated: 2025/07/18 03:07:26 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_cmd_errors(char *cmd_path)
{
	if (cmd_path)
	{
		ft_putstr_fd_up("minishell: 1", 2);
		ft_putstr_fd_up(cmd_path, 2);
		ft_putstr_fd_up(": execution failed\n", 2);
	}
}

void	erro_handle(char *cmd, char *err_message)
{
	ft_putstr_fd_up("minishell: ", 2);
	ft_putstr_fd_up(cmd, 2);
	ft_putstr_fd_up(err_message, 2);
}

int	handle_err_file(t_file *file)
{
	ft_putstr_fd_up("minishell: ", 2);
	ft_putstr_fd_up(file->name, 2);
	ft_putstr_fd(": No such file or directory\n", 2, 0);
	return (1);
}

int	put_error_ambig(char *file_name)
{
	ft_putstr_fd_up("minishell: ", 2);
	ft_putstr_fd_up(file_name, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2, 0);
	return (1);
}
