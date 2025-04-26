#include "minishell.h"


// this the main is just for testing
int main(void)
{
    char        *input;
    t_token     *tokens;
    t_cmd       **commands;
    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            printf("exit\n");
            break ;
        }
        if (*input)
            add_history(input);
        tokens = string_tokens(input);
        if (!tokens)
        {
            free(input);
            continue ;
        }
        commands = parse_commands(tokens);
        if (commands)
        {
            int i = 0;
            while (commands[i])
            {
                printf("Command #%d:\n", i + 1);
                print_command_with_files(commands[i]);
                i++;
            }
            free_cmd(*commands);
        }
        free_tokens(tokens, input); 
    }
    return (0);
}
