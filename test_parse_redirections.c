/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parse_redirections.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: test <test@test.com>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 00:00:00 by test              #+#    #+#             */
/*   Updated: 2024/12/16 00:00:00 by test             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Simple test to check if the parse_redirections function works
int main(void)
{
    t_cmd *cmd;
    t_token *tokens;
    
    // Initialize a test command
    cmd = init_cmd();
    if (!cmd)
        return 1;
    
    // Initialize test tokens (simplified)
    tokens = malloc(sizeof(t_token));
    tokens->type = TOKEN_REDIRECT_OUT;
    tokens->value = ft_strdup(">");
    tokens->was_quoted = 0;
    tokens->next = malloc(sizeof(t_token));
    tokens->next->type = TOKEN_WORD;
    tokens->next->value = ft_strdup("test\2file");
    tokens->next->was_quoted = 1;
    tokens->next->next = NULL;
    
    // Allocate files array
    cmd->files = malloc(sizeof(t_file) * 2);
    cmd->file_count = 0;
    
    // Test parse_redirections
    printf("Testing parse_redirections...\n");
    if (parse_redirections(cmd, &tokens))
    {
        printf("Success! Found %d redirection(s)\n", cmd->file_count);
        if (cmd->file_count > 0)
        {
            printf("File name: %s\n", cmd->files[0].name);
            printf("File type: %d\n", cmd->files[0].type);
            printf("Check expand: %d\n", cmd->files[0].check_expand);
        }
    }
    else
    {
        printf("Error in parse_redirections\n");
    }
    
    // Clean up
    free_cmd(cmd);
    free_tokens(tokens, NULL);
    
    return 0;
}