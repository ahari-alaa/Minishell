/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:38:24 by maskour           #+#    #+#             */
/*   Updated: 2025/07/17 22:08:08 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*merge_key_value(char *key, char *value)
{
	char	*tmp;
	char	*result;

	if (value)
	{
		tmp = ft_strjoin(key, "=");
		if (!tmp)
			return (NULL);
		result = ft_strjoin(tmp, value);
		free(tmp);
		if (!result)
			return (NULL);
		return (result);
	}
	return (ft_strdup(key));
}

void	add_env_export(t_env **env, char *key, char *value)
{
	t_env	*new_node;

	if (update_exist_env(*env, key, value))
		return ;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->data_env = merge_key_value(key, value);
	if (!new_node->data_env)
	{
		free(new_node);
		return ;
	}
	new_node->next = NULL;
	append_env_node(env, new_node);
}

void	update_or_add_env(t_env **env, char *key, char *value)
{
	t_env	*current;
	int		key_len;
	char	*tmp;
	char	*add_equal;

	if (!key) return ;
	key_len = ft_strlen(key);
	current = *env;
	while (current)
	{
		if (!ft_strncmp(current->data_env, key, key_len) && current->data_env[key_len] == '=')
		{
			tmp = ft_strjoin(key, "=");
			if (!tmp) return ;
			add_equal = ft_strjoin(tmp, value);
			free(tmp);
			if (!add_equal) return ;
			free(current->data_env);
			current->data_env = add_equal;
			return;
		}
		current = current->next;
	}
	add_env_export(env, key, value);
}

static char *build_appended_env_value(char *key, char *old_val, char *new_val)
{
	char *joined;
	char *prefix;
	char *final_val;

	joined = ft_strjoin(old_val, new_val);
	if (!joined)
		return (NULL);
	prefix = ft_strjoin(key, "=");
	if (!prefix) {
		free(joined);
		return (NULL);
	}
	final_val = ft_strjoin(prefix, joined);
	free(prefix);
	free(joined);
	return (final_val);
}

int	update_env_value(t_env *current, char *key, char *value)
{
	char	*old_val;
	char	*new_data;

	old_val = ft_strchr(current->data_env, '=');
	if (!old_val)
		return (0);
	old_val++;
	new_data = build_appended_env_value(key, old_val, value);
	if (!new_data)
		return (0);
	free(current->data_env);
	current->data_env = new_data;
	return (1);
}
