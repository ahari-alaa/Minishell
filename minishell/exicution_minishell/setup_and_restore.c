/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_and_restore.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:09:54 by maskour           #+#    #+#             */
/*   Updated: 2025/07/17 17:41:07 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_terminal_and_signals(struct termios *original_termios)
{
	if (tcgetattr(STDIN_FILENO, original_termios) == -1)
		perror("tcgetattr failed");
	ignore_sigint();
}

void	restore_terminal_and_signals(struct termios *original_termios)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, original_termios) == -1)
		perror("tcsetattr failed");
	restore_sigint();
}
