/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 03:04:25 by ahari             #+#    #+#             */
/*   Updated: 2025/04/20 06:58:06 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static char *get_quoted_string(const char *str, int *index, char quote_char)
{
    int start = *index + 1;
    int len = 0;

    while (str[start + len] && str[start + len] != quote_char)
        len++;

    if (str[start + len] != quote_char)
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `%c'\n", quote_char);
        return NULL;
    }
    char *val = malloc(len + 1);
    if (!val)
        return NULL;

    strncpy(val, &str[start], len);
    val[len] = '\0';
    *index = start + len + 1;
    return val;
}

t_token *string_tokens(char *str)
{
    t_token *head = NULL;
    int i = 0;
    int start;
    char *val;
    while (str[i])
    {
        while (ft_isspace(str[i]))
            i++;
        if (!str[i])
            break;
        start = i;
        val = NULL;
        t_token_type type;

        if (str[i] == '\'' || str[i] == '\"')
        {
            val = get_quoted_string(str, &i, str[i]);
            if (!val)
                return (free_tokens(head, NULL), NULL);
            type = TOKEN_WORD;
        }
        // Handle operators
        else if (is_operator(str[i]))
        {
            if ((str[i] == '>' || str[i] == '<') && str[i] == str[i+1])
            {
                val = strndup(&str[i], 2);  // Handle >> or <<
                i += 2;
            }
            else
                val = strndup(&str[i++], 1);  // Handle >, <, or |
            type = get_token_type(val);
        }
        else
        {
            while (str[i] && !ft_isspace(str[i]) && !is_operator(str[i]))
                i++;
            val = strndup(&str[start], i - start);
            type = TOKEN_WORD;
        }
        if (val)
        {
            t_token *new = new_token(val, type);
            if (!new)
                return (free_tokens(head, val), NULL);
            add_token(&head, new);
        }
    }

    return head;
}
