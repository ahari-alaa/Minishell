/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:11:18 by maskour           #+#    #+#             */
/*   Updated: 2025/07/19 12:50:00 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

void	free_char_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	free_env_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		env_list = current->next;
		free(current->data_env);
		free(current);
		current = env_list;
	}
}

void	remove_env_key(t_env **env, const char *key)
{
	t_env	*current;
	t_env	*prev;
	int		key_len;

	key_len = ft_strlen(key);
	current = *env;
	prev = NULL;
	while (current)
	{
		if (!ft_strncmp(current->data_env, key, key_len) && \
			(current->data_env[key_len] == '=' || \
					current->data_env[key_len] == '\0'))
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->data_env);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
