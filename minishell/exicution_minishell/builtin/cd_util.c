/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:51:04 by maskour           #+#    #+#             */
/*   Updated: 2025/07/17 21:55:00 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	update_env_var(t_env *data_env, char *key, char *dest)
{
	t_env	*current;
	char	*new_path;

	current = data_env;
	while (current)
	{
		if (ft_strncmp(current->data_env, key, ft_strlen(key)) == 0)
		{
			new_path = ft_strjoin(key, dest);
			if (!new_path)
				return ;
			free(current->data_env);
			current->data_env = new_path;
			return ;
		}
		current = current->next;
	}
}

char	*validate_path(t_cmd *cmd_path, t_env *data_env, t_shell *shell_ctx)
{
	char	*path;

	if (!cmd_path->cmd[1] || !ft_strcmp(cmd_path->cmd[1], "~"))
	{
		path = search_env(data_env, "HOME");
		if (!path)
		{
			ft_putstr_fd_up("minishell: cd: HOME not set\n", 2);
			shell_ctx->exit_status = 1;
			return (NULL);
		}
	}
	else if (!ft_strcmp(cmd_path->cmd[1], "-"))
	{
		path = search_env(data_env, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd_up("minishell: cd: OLDPWD not set\n", 2);
			shell_ctx->exit_status = 1;
			return (NULL);
		}
	}
	else
		path = cmd_path->cmd[1];
	return (path);
}

void	check_getcwd(char *oldpwd_update, t_cmd *cmd_path,
				t_shell *shell_ctx)
{
	if (!getcwd(oldpwd_update, PATH_MAX))
	{
		if (ft_strncmp(cmd_path->cmd[1], ".", 2) == 0)
		{
			if (access(".", F_OK | X_OK) == 0)
			{
				ft_putstr_fd_up("minishell: cd: ", 2);
				ft_putstr_fd_up("error retrieving current directory:", 2);
				ft_putstr_fd_up(": getcwd: cannot access parent directories\n",
					2);
			}
		}
		shell_ctx->exit_status = 1;
	}
}

int	access_path(char *path, t_shell *shell_ctx)
{
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd_up("minishell: cd: ", 2);
		ft_putstr_fd_up(path, 2);
		ft_putstr_fd_up(": No such file or directory\n", 2);
		shell_ctx->exit_status = 1;
		return (0);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd_up("minishell: cd: ", 2);
		ft_putstr_fd_up(path, 2);
		ft_putstr_fd_up(": Permission denied\n", 2);
		shell_ctx->exit_status = 1;
		return (0);
	}
	return (1);
}

void	update(t_env *data_env, char *oldpwd_update, char *pwd_update)
{
	add_oldpwd(data_env);
	update_env_var(data_env, "OLDPWD=", oldpwd_update);
	update_env_var(data_env, "PWD=", pwd_update);
}
