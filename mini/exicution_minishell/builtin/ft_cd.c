/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:49:15 by maskour           #+#    #+#             */
/*   Updated: 2025/07/17 17:53:45 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	check_cmd(t_cmd *cmd_path, t_shell *shell_ctx)
{
	if (!cmd_path || !cmd_path->cmd || !cmd_path->cmd[0])
	{
		shell_ctx->exit_status = 1;
		return (0);
	}
	return (1);
}

static int	checkaccec_cmd(t_shell *shell_ctx)
{
	if (access(".", F_OK | X_OK) != 0)
	{
		perror("minishell: cd: error retrieving current directory");
		shell_ctx->exit_status = 1;
		return (0);
	}
	return (1);
}

static int	check_path_2(char *path, char *pwd_update, t_shell *shell_ctx)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd_up("minishell: cd: ", 2);
		perror(path);
		shell_ctx->exit_status = 1;
		return (0);
	}
	if (!getcwd(pwd_update, PATH_MAX))
	{
		shell_ctx->exit_status = 1;
		return (0);
	}
	return (1);
}

static void	check_update_pwd(t_cmd *cmd_path, char *pwd_update)
{
	if (cmd_path->cmd[1] && !ft_strcmp(cmd_path->cmd[1], "-"))
	{
		ft_putstr_fd_up(pwd_update, 1);
		ft_putstr_fd_up("\n", 1);
	}
}

t_env	*ft_cd(t_cmd **cmd, t_env *data_env, t_shell *shell_ctx)
{
	char	pwd_update[PATH_MAX];
	char	oldpwd_update[PATH_MAX];
	char	*path;
	t_cmd	*cmd_path;

	cmd_path = *cmd;
	if (!check_cmd(cmd_path, shell_ctx))
		return (data_env);
	if (!checkaccec_cmd(shell_ctx))
		return (data_env);
	check_getcwd(oldpwd_update, cmd_path, shell_ctx);
	path = validate_path(cmd_path, data_env, shell_ctx);
	if (!path)
		return (data_env);
	if (!access_path(path, shell_ctx))
		return (data_env);
	if (!check_path_2(path, pwd_update, shell_ctx))
		return (data_env);
	update(data_env, oldpwd_update, pwd_update);
	check_update_pwd(cmd_path, pwd_update);
	shell_ctx->exit_status = 0;
	return (data_env);
}
