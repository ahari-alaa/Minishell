/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:38:48 by maskour           #+#    #+#             */
/*   Updated: 2025/07/17 22:11:47 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_key(char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
	if (!ft_isalpha_up(*key) && *key != '_')
		return (0);
	i = 0;
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
	}
	return (1);
}

int	is_exist(t_env *env, char *key)
{
	t_env	*current;
	int		key_len;

	if (!key)
		return (0);
	current = env;
	key_len = ft_strlen(key);
	while (current)
	{
		if (!ft_strncmp(current->data_env, key, key_len) && \
			(current->data_env[key_len] == '=' || \
				current->data_env[key_len] == '\0'))
			return (1);
		current = current->next;
	}
	return (0);
}

void	extra_key_value(char *input, char **key, char **value)
{
	char	*input_copy;
	char	*equal;

	*key = NULL;
	*value = NULL;
	if (!input)
		return ;
	input_copy = ft_strdup(input);
	if (!input_copy)
		return ;
	equal = ft_strchr(input_copy, '=');
	if (equal)
	{
		if (equal > input_copy && *(equal - 1) == '+')
			*(equal - 1) = '\0';
		else
			*equal = '\0';
		*key = ft_strdup(input_copy);
		*value = ft_strdup(equal + 1);
	}
	else 
	{
		*key = ft_strdup(input_copy);
		*value = NULL;
	}
	free(input_copy);
}

void	append_env_node(t_env **env, t_env *new_node)
{
	t_env	*last;
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	last = *env;
	while (last->next)
		last = last->next;
	last->next = new_node;
}

int	update_exist_env(t_env *env, char *key, char *value)
{
	t_env	*current = env;

	while (current)
	{
		if (!strncmp(current->data_env, key, strlen(key)) && \
			(current->data_env[strlen(key)] == '=' || current->data_env[strlen(key)] == '\0')) 
		{
			free(current->data_env);
			if (value)
			{
				char *tmp = ft_strjoin(key, "=");
				if (!tmp) 
					return (1);
				current->data_env = ft_strjoin(tmp, value);
				free(tmp);
			}
			else 
				current->data_env = strdup(key);
			return (1); 
		}
		current = current->next;
	}
	return (0);
}
