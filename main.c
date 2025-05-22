#include "minishell.h"


// this the main is just for testing
int main(void)
{
    char        *input;
    t_token     *tokens;
     t_cmd       *commands;
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
        tokens = check_quoted(input);
        if (!tokens)
            continue ;
        commands = parse_commands(tokens);
        if (!commands)
        {
            free_tokens(tokens, input);
            continue ;
        }
        commands = expand_cmd_list(commands);
        if (!commands)
        {
            free_cmd_list(commands);
            free_tokens(tokens, input);
            continue ;
        }
        commands = unquote_cmd_list(commands);
        if (!commands)
        {
            free_cmd_list(commands);
            free_tokens(tokens, input);
            continue ;
        }
        if (commands)
        {
            t_cmd *current = commands;
            int i = 1;
            
            while (current)
            {
                printf("Command #%d:\n", i);
                print_command_with_files(current);
                current = current->next;
                i++;
            }
        }
        if(commands)
            free_cmd_list(commands);
        free_tokens(tokens, input);
    }
    return (0);
}
