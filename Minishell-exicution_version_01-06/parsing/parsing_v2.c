/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_v2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:15:00 by ahari             #+#    #+#             */
/*   Updated: 2025/06/27 21:17:42 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// #include "../minishell.h"

// t_token *find_previous_token(t_token *head, t_token *target)
// {
//     if (!head || head == target)
//         return NULL;
//     t_token *current = head;
//     while (current->next && current->next != target)
//         current = current->next;
//     if (current->next == target)
//         return (current);
//     else
//         return (NULL);
// }

// static t_token  *find_command_token(t_token *head, t_token *current)
// {
// 	t_token *temp;
// 	t_token	*cmd_token;

// 	temp = head;
// 	cmd_token = NULL;
// 	while (temp && temp != current)
// 	{
// 		if (temp->type == TOKEN_PIPE || temp->type == TOKEN_SEMICOLON)
// 			cmd_token = NULL;
// 		else if (temp->type == TOKEN_WORD)
// 		{
// 			if (!cmd_token)
// 				cmd_token = temp;
// 		}
// 		temp = temp->next;
// 	}
// 	return (cmd_token);
// }

// static int	has_quotes_before_assignment(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '\'' || str[i] == '"')
// 			return (1);
// 		if (str[i] == '=')
// 			break ;
// 		i++;
// 	}
// 	return (0);
// }

// static int	validate_export_assignment(t_token *current)
// {
// 	if (has_quotes_before_assignment(current->value))
// 		return (2);
// 	if (ft_strchr(current->value, '='))
// 		return (1);
// 	return (0);
// }

// int	is_export_assignment(t_token *head, t_token *current)
// {
// 	t_token	*cmd_token;

// 	if (!head || !current)
// 		return (0);
// 	cmd_token = find_command_token(head, current);
// 	if (cmd_token && ft_strcmp(cmd_token->value, "export") == 0)
// 		return (validate_export_assignment(current));
// 	return (0);
// }
