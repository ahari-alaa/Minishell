/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:22 by maskour           #+#    #+#             */
/*   Updated: 2025/07/19 12:50:56 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	nb_args(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	return (i);
}

static int	check_new_line(char *cmd)
{
	int	i;

	if (cmd[0] != '-')
		return (0);
	i = 1;
	if (!cmd[i])
		return (0);
	while (cmd[i])
	{
		if (cmd[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(t_cmd **cmd, t_shell *shell_ctx)
{
	int		i;
	int		n_nb;
	t_cmd	*current_cmd;

	i = 1;
	n_nb = 0;
	current_cmd = *cmd;
	if (nb_args(current_cmd->cmd) > 1)
	{
		while (current_cmd->cmd[i] && check_new_line(current_cmd->cmd[i]))
		{
			n_nb = 1;
			i++;
		}
		while (current_cmd->cmd[i])
		{
			write(1, current_cmd->cmd[i], ft_strlen(current_cmd->cmd[i]));
			if (current_cmd->cmd[i + 1])
				write(1, " ", 1);
			i++;
		}
	}
	if (n_nb == 0)
		write(1, "\n", 1);
	shell_ctx->exit_status = 0;
}
