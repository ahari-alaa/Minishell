/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:50:12 by maskour           #+#    #+#             */
/*   Updated: 2025/07/18 23:03:31 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	append_value(t_env **env, char *key, char *value)
{
	t_env	*current;
	int		key_len;

	if (!key || !value)
		return ;
	current = *env;
	key_len = ft_strlen(key);
	while (current)
	{
		if (!ft_strncmp(current->data_env, key, key_len) && \
			current->data_env[key_len] == '=')
		{
			if (update_env_value(current, key, value))
				return ;
			else
				return ;
		}
		current = current->next;
	}
	add_env_export(env, key, value);
}

static int	handel_append(t_env **env, char *cmd)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	extra_key_value(cmd, &key, &value);
	if (!is_valid_key(key))
	{
		ft_putstr_fd_up("minishell: export: ", 2);
		ft_putstr_fd_up(cmd, 2);
		ft_putstr_fd_up(": not a valid identifier\n", 2);
		(free(key), free(value));
		return (1);
	}
	append_value(env, key, value);
	return (free(key), free(value), 0);
}

static int	handle_assigmnet(t_env **env, char *cmd)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	extra_key_value(cmd, &key, &value);
	if (!is_valid_key(key))
	{
		ft_putstr_fd_up("minishell: export: ", 2);
		ft_putstr_fd_up(cmd, 2);
		ft_putstr_fd_up(": not a valid identifier\n", 2);
		(free(key), free(value));
		return (1);
	}
	update_or_add_env(env, key, value);
	return (free(key), free(value), 0);
}

static int	handle_export(t_env **env, char *cmd)
{
	char	*empty;
	char	*key;

	empty = "";
	key = NULL;
	extra_key_value(cmd, &key, &empty);
	if (!is_valid_key(key))
	{
		ft_putstr_fd_up("minishell: export: ", 2);
		ft_putstr_fd_up(cmd, 2);
		ft_putstr_fd_up(": not a valid identifier\n", 2);
		free(key);
		return (1);
	}
	if (!is_exist(*env, key))
		add_env_export(env, key, NULL);
	return (free(key), 0);
}

void	ft_export(t_cmd **cmd_ptr, t_env **envp, t_shell *shell)
{
	t_cmd	*cmd;
	int		i;
	char	*assignment;
	char	*append;
	int		stat;

	i = 1;
	stat = 0;
	cmd = *cmd_ptr;
	if (first_sort_env(cmd, envp, shell))
		return ;
	while (cmd->cmd[i])
	{
		assignment = ft_strchr(cmd->cmd[i], '=');
		append = ft_strstr(cmd->cmd[i], "+=");
		if (append)
			stat = handel_append(envp, cmd->cmd[i]);
		else if (assignment)
			stat = handle_assigmnet(envp, cmd->cmd[i]);
		else
			stat = handle_export(envp, cmd->cmd[i]);
		i++;
	}
	shell->exit_status = stat;
}
