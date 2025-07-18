/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:51:21 by maskour           #+#    #+#             */
/*   Updated: 2025/07/17 22:06:28 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_sort_env(t_env *env)
{
	t_env	*current;
	char	*eq;
	int		key_len;

	current = env;
	while (current)
	{
		eq = ft_strchr(current->data_env, '=');
		if (eq)
		{
			key_len = eq - current->data_env;
			printf("declare -x ");
			fwrite(current->data_env, 1, key_len, stdout);
			printf("=\"%s\"\n", eq + 1);
		}
		else
			printf("declare -x %s\n", current->data_env);
		current = current->next;
	}
}
static t_env	*get_last_unsorted(t_env *start, t_env *end)
{
	t_env	*ptr;

	ptr = start;
	while (ptr->next != end)
		ptr = ptr->next;
	return (ptr);
}

static int	swap_pass(t_env *start, t_env *end)
{
	t_env	*ptr;
	char	*tmp;
	int		swapped;

	swapped = 0;
	ptr = start;
	while (ptr->next != end)
	{
		if (ft_strcmp(ptr->data_env, ptr->next->data_env) > 0)
		{
			tmp = ptr->data_env;
			ptr->data_env = ptr->next->data_env;
			ptr->next->data_env = tmp;
			swapped = 1;
		}
		ptr = ptr->next;
	}
	return (swapped);
}

static t_env	*sort_env(t_env *env)
{
	t_env	*lptr;
	int		swapped;

	if (!env || !env->next)
		return (env);
	lptr = NULL;
	swapped = 1;
	while (swapped)
	{
		swapped = swap_pass(env, lptr);
		lptr = get_last_unsorted(env, lptr);
	}
	return (env);
}

void	sort_and_display_env(t_env **envp)
{
	*envp = sort_env(*envp);
	print_sort_env(*envp);
}
