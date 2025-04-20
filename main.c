#include "minishell.h"


// this the main is just for testing
int main(void)
{
    char        *input;
    t_token    *tokens;

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
        print_tokens(tokens);
        free_tokens(tokens, input); 
    }
    return (0);
}