/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:08:30 by maskour           #+#    #+#             */
/*   Updated: 2025/07/18 20:18:27 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



static void	main_loop(t_env *env_list, t_shell *shell_ctx)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*commands;
	char	**env_table;

	while (1)
	{
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
		free_array(env_table);
		exicut(&commands, &env_list, shell_ctx);
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
	env_list = file_inv(env);
	shell_ctx = init_shell_ctx();
	signal(SIGINT, handler_sig);
	signal(SIGQUIT, handler_sig);
	main_loop(env_list, shell_ctx);
	free_env_list(env_list);
	free(shell_ctx);
	return (0);
}
