// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   split_with_quotes.c                               :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: username <username@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/01/01 00:00:00 by username          #+#    #+#             */
// /*   Updated: 2024/01/01 00:00:00 by username         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../minishell.h"

// static void	advance_through_word(char *str, int *i, int *in_quotes,
// 		char *quote_char)
// {
// 	while (str[*i])
// 	{
// 		if (str[*i] == '\'' || str[*i] == '"')
// 		{
// 			if (!*in_quotes)
// 			{
// 				*in_quotes = 1;
// 				*quote_char = str[*i];
// 			}
// 			else if (str[*i] == *quote_char)
// 			{
// 				*in_quotes = 0;
// 				*quote_char = 0;
// 			}
// 			(*i)++;
// 		}
// 		else if (!*in_quotes && ft_isspace(str[*i]))
// 			break ;
// 		else
// 			(*i)++;
// 	}
// }

// static int	count_words_with_quotes(char *str)
// {
// 	int		i;
// 	int		count;
// 	int		in_quotes;
// 	char	quote_char;

// 	i = 0;
// 	count = 0;
// 	in_quotes = 0;
// 	quote_char = 0;
// 	while (str[i])
// 	{
// 		while (ft_isspace(str[i]))
// 			i++;
// 		if (!str[i])
// 			break ;
// 		count++;
// 		advance_through_word(str, &i, &in_quotes, &quote_char);
// 	}
// 	return (count);
// }

// static int	extract_words(char *str, char **result)
// {
// 	int		i;
// 	int		start;
// 	int		count;
// 	int		in_quotes;
// 	char	quote_char;

// 	i = 0;
// 	count = 0;
// 	in_quotes = 0;
// 	quote_char = 0;
// 	while (str[i])
// 	{
// 		while (ft_isspace(str[i]))
// 			i++;
// 		if (!str[i])
// 			break ;
// 		start = i;
// 		advance_through_word(str, &i, &in_quotes, &quote_char);
// 		result[count] = ft_strndup(str + start, i - start);
// 		if (!result[count])
// 			return (0);
// 		count++;
// 	}
// 	return (1);
// }

// char	**split_with_quotes(char *str)
// {
// 	char	**result;
// 	int		word_count;

// 	if (!str || !*str)
// 		return (NULL);
// 	word_count = count_words_with_quotes(str);
// 	if (word_count == 0)
// 		return (NULL);
// 	result = malloc(sizeof(char *) * (word_count + 1));
// 	if (!result)
// 		return (NULL);
// 	if (!extract_words(str, result))
// 	{
// 		free_array(result);
// 		return (NULL);
// 	}
// 	result[word_count] = NULL;
// 	return (result);
// }