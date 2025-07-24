/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:39:24 by maskour           #+#    #+#             */
/*   Updated: 2025/07/21 19:30:41 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env	*new_env(char *data_env)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->data_env = ft_strdup(data_env);
	if (!node->data_env)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

static t_env	*push_env_node(t_env **tail, t_env *env_list, char *entry)
{
	t_env	*new;

	new = new_env(entry);
	if (!new)
	{
		free_env_list(env_list);
		return (NULL);
	}
	if (*tail)
	{
		(*tail)->next = new;
		*tail = new;
	}
	return (new);
}

static t_env	*add_pwd_entry(t_env *env_list, t_env **tail)
{
	char	cwd[1024];
	char	pwd_entry[1100];

	if (getcwd(cwd, sizeof(cwd)))
	{
		strcpy(pwd_entry, "PWD=");
		strcat(pwd_entry, cwd);
		env_list = push_env_node(tail, NULL, pwd_entry);
		if (!env_list)
			return (NULL);
		*tail = env_list;
	}
	return (env_list);
}

static t_env	*default_env(t_env *env_list)
{
	t_env	*tail;

	tail = NULL;
	env_list = add_pwd_entry(env_list, &tail);
	if (!env_list)
	{
		return (NULL);
	}
	tail = push_env_node(&tail, env_list, "SHLVL=1");
	if (!tail)
		return (NULL);
	tail = push_env_node(&tail, env_list, "_=/usr/bin/env");
	if (!tail)
		return (NULL);
	tail = push_env_node(&tail, env_list, \
		"PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	if (!tail)
		return (NULL);
	return (env_list);
}

t_env	*file_inv(char **env)
{
	t_env	*env_list;
	t_env	*new;
	int		i;

	env_list = NULL;
	new = NULL;
	if (!env || !env[0])
		return (default_env(env_list));
	i = 0;
	while (env[i])
	{
		new = new_env(env[i]);
		if (!new)
		{
			free_env_list(env_list);
			return (NULL);
		}
		add_env(&env_list, new);
		i++;
	}
	remove_env_key(&env_list, "OLDPWD");
	return (env_list);
}
