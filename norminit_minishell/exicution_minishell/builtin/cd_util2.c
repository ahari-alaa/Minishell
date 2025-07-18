/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_util2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:53:28 by maskour           #+#    #+#             */
/*   Updated: 2025/07/17 21:58:11 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	add_oldpwd(t_env *data_env)
{
	t_env	*curreent;
	t_env	*last;
	char	*new_path;
	t_env	*new_node;

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
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (free(new_path));
	new_node->data_env = new_path;
	new_node->next = NULL;
	if (last)
		last->next = new_node;
}
