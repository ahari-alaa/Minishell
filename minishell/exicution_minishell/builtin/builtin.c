/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:48:55 by maskour           #+#    #+#             */
/*   Updated: 2025/07/23 19:17:42 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_builtin(char *command)
{
	if (!command)
		return (0);
	return (ft_strcmp(command, "cd") == 0 || \
		ft_strcmp(command, "echo") == 0 || \
		ft_strcmp(command, "exit") == 0 || \
		ft_strcmp(command, "export") == 0 || \
		ft_strcmp(command, "unset") == 0 || \
		ft_strcmp(command, "env") == 0 || \
		ft_strcmp(command, "pwd") == 0);
}

t_env	*execut_bultin(t_cmd **cmd, t_env *env_list, t_shell *shell, int i)
{
	if (ft_strcmp(cmd[0]->cmd[0], "cd") == 0)
		env_list = ft_cd(cmd, env_list, shell);
	else if (ft_strcmp(cmd[0]->cmd[0], "echo") == 0)
		ft_echo(cmd, shell);
	else if (ft_strcmp(cmd[0]->cmd[0], "exit") == 0)
		ft_exit(cmd, shell, i);
	else if (ft_strcmp(cmd[0]->cmd[0], "export") == 0)
		ft_export(cmd, &env_list, shell);
	else if (ft_strcmp(cmd[0]->cmd[0], "unset") == 0)
		env_list = ft_unset(cmd, env_list, shell);
	else if (ft_strcmp(cmd[0]->cmd[0], "env") == 0)
		ft_env(env_list, shell);
	else if (ft_strcmp(cmd[0]->cmd[0], "pwd") == 0)
		ft_pwd(shell, env_list);
	return (env_list);
}
