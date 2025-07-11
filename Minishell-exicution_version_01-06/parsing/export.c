/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: username <username@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by username          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by username         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_var_exp_assignment(const char *str)
{
	const char	*eq;
	const char	*p;
	int			check;

	p = str;
	eq = ft_strchr(str, '=');
	if (!eq || eq == str)
		return (0);
	if (ft_isdigit(*p))
		check = -1;
	while (*p && p < eq)
	{
		if (*p == '\'' || *p == '"' || *p == '$')
		{
			check = -1;
			break;
		}
		p++;
	}
	if (str[0] == '\'' || str[0] == '"')
		return (0);
	if (*(eq + 1) == '\"' || *(eq + 1) == '\'')
		return (1);
	if (*(eq + 1) == '\"' && *(eq + 2) == '$')
		return (1);
	if (*(eq + 1) == '$' && check != -1)
		return (1);
	return (0);
}

int	is_export_assignment(t_token *head, t_token *current)
{
	t_token	*prev;

	prev = find_previous_token(head, current);
	if (!head || !current)
		return (0);
	if (prev && prev->was_quoted == 0)
	{
		if(is_var_exp_assignment(current->value) == 1)
			return (1);
	}
	return (0);
}
