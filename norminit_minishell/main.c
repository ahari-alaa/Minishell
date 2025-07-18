/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:08:30 by maskour           #+#    #+#             */
/*   Updated: 2025/07/18 02:28:19 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell *g_shell_ctx = NULL;

void handler_sig_1(int signal)
{

	if (signal == SIGINT)
	{
		write(1,"\n",1);
  		rl_on_new_line();
  		rl_replace_line("", 0);
  		rl_redisplay();
		if (g_shell_ctx)
			g_shell_ctx->exit_status = 1;
	}
}
static int validate_terminal(void)
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
static void start_program(t_env *env_list, t_shell *shell_ctx)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*commands;
	char	**env_table;

	while (1)
	{
		signal(SIGINT, handler_sig_1); 
		signal(SIGQUIT, handler_sig); 
		input = readline("minishell$ ");
		if (!input)
		{     
			write(1 ,"exit\n", 5);
			(free_env_list(env_list), exit(shell_ctx->exit_status));
		}
		if (input[0])
			add_history(input);
		env_table = convert(env_list);
				tokens = check_quoted(input, shell_ctx, env_table);
		if (!tokens)
		{
			(free (input), free_char_array(env_table));
			continue ;
		}    
		commands = parse_commands(tokens , shell_ctx);
		if (!commands)
		{
			(free_tokens(tokens, input), free_char_array(env_table));
			continue ;
		}
		if (count_herdoc(commands) > 16)
		{
			write(2, "minishell: too many here-documents\n", 36);
			(free_tokens(tokens, input), free_cmd_list(commands));
			(free_char_array(env_table), free_env_list(env_list));
			(free(shell_ctx), exit(2));
		}
		free_char_array(env_table);
		exicut(&commands, &env_list, shell_ctx);
		if(commands)
			free_cmd_list(commands);
		free_tokens(tokens, input);
	}
}

int	main(int ac,char **av,char **env)
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
