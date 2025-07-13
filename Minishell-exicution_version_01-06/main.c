/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:08:30 by maskour           #+#    #+#             */
/*   Updated: 2025/07/13 21:48:57 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

     
#include "minishell.h"

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    char        *input;
    t_token     *tokens;
    t_cmd       *commands;
    t_env       *env_list;
    char        **env_table;
    t_shell     *shell_ctx;

    // Init env and shell context
    env_list = file_inv(env);
    shell_ctx = malloc(sizeof(t_shell));
    if (!shell_ctx)
        exit(1);
    shell_ctx->exit_status = 0;

    signal(SIGINT, handler_sig); 
    signal(SIGQUIT, handler_sig); 

    // Uncomment for debug memory check on exit
    // atexit(ff);

    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            write(1, "exit\n", 5);
            break;
        }

        if (input[0])
            add_history(input);

        
        env_table = convert(env_list);
        tokens = check_quoted(input, shell_ctx, env_table);
        // print_tokens(tokens);
        free(input);
        if (!tokens)
        {
            free(NULL);
            free_array(env_table);
            continue;
        }
        commands = parse_commands(tokens, shell_ctx);

        if (!commands)
        {
            free_tokens(tokens, NULL);
            free_array(env_table); // 🧼 FIXED: memory leak
            continue;
        }

        if (count_herdoc(commands) > 16)
        {
            write(2, "minishell: too many here-documents\n", 36);
            shell_ctx->exit_status = 2;
            free_tokens(tokens, NULL);
            free_cmd_list(commands);
            free_array(env_table);
            break;
        }
        free_array(env_table);
        exicut(&commands, &env_list, shell_ctx);
        if (commands)
            free_cmd_list(commands);
        free_tokens(tokens, NULL);
    }
    free_env_list(env_list);
    free(shell_ctx);
    return 0;
}
