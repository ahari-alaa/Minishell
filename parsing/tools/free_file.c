/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:04:47 by ahari             #+#    #+#             */
/*   Updated: 2025/04/20 06:09:11 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char *ft_strndup(const char *s, size_t n)
{

    char *dup = malloc(n + 1);
    if (!dup)
        return NULL;

    size_t i = 0;
    while (i < n && s[i] != '\0')
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';

    return dup;
}

void free_tokens(t_token *tokens, char *input)
{
    t_token *temp;

    while (tokens)
    {
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
    free(input);
}
char    *ft_strdup(const char *s1)
{
    size_t len = 0;
    char *dup;
    size_t i;

    if (!s1)
        return (NULL);
    while (s1[len])
        len++;
    dup = (char *)malloc((len + 1) * sizeof(char));
    if (!dup)
        return (NULL);
    i = 0;
    while (s1[i])
    {
        dup[i] = s1[i];
        i++;
    }
    dup[i] = '\0';
    return dup;
}
/*-------------------for testing----------------------*/

void print_token_type(t_token_type type)
{
    switch (type)
    {
        case TOKEN_WORD:
            printf("TOKEN_WORD");
            break;
        case TOKEN_PIPE:
            printf("TOKEN_PIPE");
            break;
        case TOKEN_REDIRECT_IN:
            printf("TOKEN_REDIRECT_IN");
            break;
        case TOKEN_REDIRECT_OUT:
            printf("TOKEN_REDIRECT_OUT");
            break;
        case TOKEN_APPEND:
            printf("TOKEN_APPEND");
            break;
        case TOKEN_HEREDOC:
            printf("TOKEN_HEREDOC");
            break;
        case TOKEN_NULL:
            printf("TOKEN_NULL");
            break;
        default:
            printf("UNKNOWN_TOKEN");
            break;
    }
}

void print_tokens(t_token *head)
{
    t_token *current = head;

    while (current)
    {
        printf("Token: %s  Type: ", current->value);
        print_token_type(current->type);
        printf("\n");
        current = current->next;
    }
}
