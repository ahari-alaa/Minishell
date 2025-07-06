// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   process_tokens.c                                   :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/07/01 17:17:31 by ahari             #+#    #+#             */
// /*   Updated: 2025/07/06 00:04:04 by ahari            ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../minishell.h"

// char *ft_strjoin3(char const *s1, char const *s2, char const *s3)
// {
//     char    *result;
//     size_t  len1;
//     size_t  len2;
//     size_t  len3;
//     size_t  total_len;

//     if (!s1 || !s2 || !s3)
//         return (NULL);
//     len1 = ft_strlen(s1);
//     len2 = ft_strlen(s2);
//     len3 = ft_strlen(s3);
//     total_len = len1 + len2 + len3 + 1;
//     result = (char *)malloc(total_len * sizeof(char));
//     if (!result)
//         return (NULL);
//     ft_strlcpy(result, s1, len1 + 1);
//     ft_strlcpy(result + len1, s2, len2 + 1);
//     ft_strlcpy(result + len1 + len2, s3, len3 + 1);
//     return (result);
// }

// void export_one_case(char *value, t_token *head)
// {
//     char *equal_sign;
//     char *var_part;
//     char *val_part;
//     char *new_value;
//     int needs_quotes = 0;

//     if (!value || !head)
//         return;
//     equal_sign = ft_strchr(value, '=');
//     if (!equal_sign)
//         return; 
//     var_part = ft_strndup(value, equal_sign - value);
//     val_part = ft_strdup(equal_sign + 1);
//     if (!var_part || !val_part)
//     {
//         free(var_part);
//         free(val_part);
//         return;
// 	}
//     int has_empty_quotes = (var_part[0] == '\'' && var_part[1] == '\'');
//     if (ft_strchr(val_part, '$') && 
//         !is_quoted(val_part, val_part) && 
//         !has_empty_quotes)
//     {
//         needs_quotes = 1;
//     }
//     if (needs_quotes)
//     {
//         new_value = ft_strjoin3(var_part, "=\"", val_part);
//         if (new_value)
//         {
//             new_value = ft_strjoin(new_value, "\"");
//             free(value);
//             head->value = new_value;
//             head->was_quoted = 1;
//         }
//     }
//     else
//     {
//         new_value = ft_strjoin3(var_part, "=", val_part);
//         if (new_value)
//         {
//             free(value);
//             head->value = new_value;
//         }
//     }
//     free(var_part);
//     free(val_part);
// }