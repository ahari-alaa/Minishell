/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:08:30 by maskour           #+#    #+#             */
/*   Updated: 2025/07/24 11:10:55 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*g_shell_ctx = NULL;

void	handler_sig_1(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		if (g_shell_ctx)
			g_shell_ctx->exit_status = 1;
	}
}

static int	validate_terminal(void)
{
	char	*ptr;

	if (!isatty(1) || !isatty(0))
	{
		write(2, "minishell: not a terminal\n", 26);
		return (0);
	}
	ptr = getcwd(NULL, 0);
	if (!ptr)
	{
		write(2, "minishell: getcwd failed\n", 26);
		return (0);
	}
	free(ptr);
	return (1);
}

static t_shell	*init_shell_context(void)
{
	t_shell	*ctx;

	ctx = malloc(sizeof(t_shell));
	if (!ctx)
		return (NULL);
	rl_catch_signals = 0;
	ctx->exit_status = 0;
	return (ctx);
}

static void	start_program(t_env *env_list, t_shell *shell_ctx)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*commands;
	char	**env_table;

	while (1)
	{
		(signal(SIGINT, handler_sig), signal(SIGQUIT, handler_sig));
		input = readline("minishell$ ");
		if (!input)
			return (ft_putstr_fd_up("exit\n", 2));
		if (input[0])
			add_history(input);
		env_table = convert(env_list);
		if (handle_input(input, env_table, &tokens, shell_ctx))
			continue ;
		if (handle_commands(tokens, shell_ctx, env_table, &commands))
			continue ;
		if (handle_here_docs(commands, shell_ctx, tokens, env_table))
			break ;
		(free_array(env_table), exicut(&commands, &env_list, shell_ctx));
		if (commands)
			free_cmd_list(commands);
		free_tokens(tokens, NULL);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*env_list;
	t_shell	*shell_ctx;

	(void)ac;
	(void)av;
	if (!validate_terminal())
		return (1);
	env_list = file_inv(env);
	shell_ctx = init_shell_context();
	if (!shell_ctx)
		exit(1);
	g_shell_ctx = shell_ctx;
	rl_catch_signals = 0;
	start_program(env_list, shell_ctx);
	free_env_list(env_list);
	free(shell_ctx);
	return (0);
}
