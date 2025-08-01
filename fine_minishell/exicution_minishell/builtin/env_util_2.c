/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:49:14 by maskour           #+#    #+#             */
/*   Updated: 2025/07/18 23:03:18 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*search_env(t_env *env, const char *key)
{
	int	len;

	len = ft_strlen(key);
	while (env)
	{
		if (!ft_strncmp(env->data_env, key, len) && env->data_env[len] == '=')
			return (env->data_env + len + 1);
		env = env->next;
	}
	return (NULL);
}

void	add_env(t_env **env_list, t_env *new_node)
{
	t_env	*tmp;

	if (!*env_list)
		*env_list = new_node;
	else
	{
		tmp = *env_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

int	first_sort_env(t_cmd *cmd, t_env **envp, t_shell *shell)
{
	if (cmd->cmd[0] && cmd->cmd[1] == NULL)
	{
		if (!cmd->cmd[1])
			sort_and_display_env(envp);
		shell->exit_status = 0;
		return (1);
	}
	return (0);
}
