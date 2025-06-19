/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:49:15 by maskour           #+#    #+#             */
/*   Updated: 2025/06/19 17:17:04 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
//UPDATE  put the PWD
//UPDate OLDPWD
//GO TO THE DIRECTION USE GETDR AND GETENV FUNCTIONS

static void add_oldpwd (t_env *data_env)
{
    t_env *curreent = data_env;
    t_env   *last = NULL;
    char *new_path;
    while (curreent)
    {
        if (ft_strstr(curreent->data_env, "OLDPWD="))
                return ;
            last = curreent;
            curreent = curreent->next;
    }
    new_path = ft_strjoin("OLDPWD", "=");
    if (!new_path)
        return;
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
    while(current)
    {
        if (ft_strstr(current->data_env,key) != NULL)
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
    char pwd_update[PATH_MAX]; // Buffer for new PWD
    char oldpwd_update[PATH_MAX]; // Buffer for old PWD
    char *path = NULL;
    t_cmd *cmd_path = *cmd;
    if (!cmd_path->cmd[1])
    {
        shell_ctx->exit_status = 0;
        return (data_env);
    }
    // Save current directory in oldpwd_update
    if (!getcwd(oldpwd_update, PATH_MAX))
    {
        perror("minishell: cd");
        shell_ctx->exit_status = 1;
        return (data_env);
    }
    // Determine path to change to
    if (cmd_path->cmd[1] && !ft_strcmp(cmd_path->cmd[1], "~"))
        {
            path = search_env(data_env,"HOME");
            printf("%s\n", path);
            if (!path)
            {
                ft_putstr_fd_up("minishell: cd: HOME not set\n",2);
                shell_ctx->exit_status = 1;
                return (data_env);
            }
        }
    else if (!ft_strcmp(cmd_path->cmd[1], "-") && cmd_path->cmd[1])
      {  
        path = search_env(data_env, "OLDPWD");
        if (!path)
        {
            ft_putstr_fd_up("minishell: cd: OLDPWD not set\n",2);
            shell_ctx->exit_status = 1;
            return (data_env);
        }
      }
    else
        path = cmd_path->cmd[1];
    // Change directory
    if (chdir(path) != 0)
    {
        ft_putstr_fd_up("minishell: cd: ", 2);
        perror(path);
        shell_ctx->exit_status = 1;
        return(data_env);
    }
    // Update environment
    if (!getcwd(pwd_update, PATH_MAX))
    {
        perror("minishell: cd: getcwd error");
        return(data_env);
    }
    // Update the oldpwd
    add_oldpwd(data_env);
    update_env_var(data_env, "OLDPWD=", oldpwd_update);
    // Update the pwd
    update_env_var(data_env, "PWD=", pwd_update);
    // Print new directory if cd -
    if (cmd_path->cmd[1] && !ft_strcmp(cmd_path->cmd[1], "-"))
    {
        ft_putstr_fd_up(pwd_update, 1); // Should print new directory (OLDPWD)
        ft_putstr_fd_up("\n", 1);
    }
    shell_ctx->exit_status = 0;
    return (data_env);
}