/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:49:15 by maskour           #+#    #+#             */
/*   Updated: 2025/07/04 20:40:53 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	add_oldpwd (t_env *data_env)
{
	t_env	*curreent;
	t_env	*last;
	char	*new_path;

	curreent = data_env;
	last = NULL;
	while (curreent)
	{
		if (ft_strstr(curreent->data_env, "OLDPWD="))
				return ;
			last = curreent;
			curreent = curreent->next;
	}
	new_path = ft_strjoin("OLDPWD", "=");
	if (!new_path)
		return ;
	t_env *new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(new_path);
		return ;
	}
	new_node->data_env = new_path;
	new_node->next = NULL;
	if (last)
		last->next = new_node;
}
static void update_env_var(t_env *data_env, char *key, char *dest)
{
    t_env *current = data_env;
    char *new_path;

    while (current)
    {
        // Use strncmp for exact prefix match
        if (ft_strncmp(current->data_env, key, ft_strlen(key)) == 0)
        {
            new_path = ft_strjoin(key, dest);
            if (!new_path)
                return;
            free(current->data_env);
            current->data_env = new_path;
            return;
        }
        current = current->next;
    }
}

t_env *ft_cd(t_cmd **cmd, t_env *data_env, t_shell *shell_ctx)
{
    char pwd_update[PATH_MAX];
    char oldpwd_update[PATH_MAX];
    char *path = NULL;
    t_cmd *cmd_path = *cmd;

    if (ft_strcmp(cmd_path->cmd[0], "cd") != 0)
        return data_env;
    if (access(".", F_OK | X_OK) != 0)
    {
        perror("minishell: cd: error retrieving current directory: getcwd: cannot access parent directories");
        shell_ctx->exit_status = 1;
        return data_env;
    }
    if (!cmd_path->cmd[1])
    {
        shell_ctx->exit_status = 0;
        return data_env;
    }
    if (!getcwd(oldpwd_update, PATH_MAX))
    {
        if (ft_strncmp(cmd_path->cmd[1],".",2) == 0)
        {
            
            if (access(".", F_OK | X_OK) == 0)
            {
                perror("minishell: cd: error retrieving current directory: getcwd: cannot access parent directories");
            }
        }
        shell_ctx->exit_status = 1;
        // return data_env;
    }
    // update PWD/OLDPWD as needed

    if (!ft_strcmp(cmd_path->cmd[1], "~"))
    {
        path = search_env(data_env, "HOME");
        if (!path)
        {
            ft_putstr_fd_up("minishell: cd: HOME not set\n", 2);
            shell_ctx->exit_status = 1;
            return data_env;
        }
    }
    else if (!ft_strcmp(cmd_path->cmd[1], "-"))
    {
        path = search_env(data_env, "OLDPWD");
        if (!path)
        {
            ft_putstr_fd_up("minishell: cd: OLDPWD not set\n", 2);
            shell_ctx->exit_status = 1;
            return data_env;
        }
    }
    else
        path = cmd_path->cmd[1];


    if (access(path, F_OK) != 0)
    {
        ft_putstr_fd_up("minishell: cd1: ", 2);
        ft_putstr_fd_up(path, 2);
        ft_putstr_fd_up(": No such file or directory\n", 2);
        shell_ctx->exit_status = 1;
        return data_env;
    }
    if (access(path,X_OK) != 0)
    {
        ft_putstr_fd_up("minishell: cd1: ", 2);
        ft_putstr_fd_up(path, 2);
        ft_putstr_fd_up(": Permission denied\n", 2);
        shell_ctx->exit_status = 1;
        return data_env;
    }
    if (chdir(path) != 0)
    {
        ft_putstr_fd_up("minishell: cd: ", 2);
        perror(path);
        shell_ctx->exit_status = 1;
        return data_env;
    }
    if (!getcwd(pwd_update, PATH_MAX))
    {
        shell_ctx->exit_status = 1;
        return data_env;
    }
    add_oldpwd(data_env);
    update_env_var(data_env, "OLDPWD=", oldpwd_update);
    update_env_var(data_env, "PWD=", pwd_update);
    if (!ft_strcmp(cmd_path->cmd[1], "-"))
    {
        ft_putstr_fd_up(pwd_update, 1);
        ft_putstr_fd_up("\n", 1);
    }
    shell_ctx->exit_status = 0;
    return data_env;
}
