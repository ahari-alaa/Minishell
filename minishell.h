/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 04:59:25 by ahari             #+#    #+#             */
/*   Updated: 2025/04/20 06:31:14 by ahari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
///the headar of all file that we will use in this project 
#include <stdio.h>  // perror()
#include <readline/readline.h> //for 
#include <readline/history.h> // for readline
#include <unistd.h> //  for getcwd() , chdir() , isatty()   q
#include <sys/stat.h> // stat() & lstat() & fstat()
#include <sys/types.h> // opendir() closedir()
#include <dirent.h> // opendir() readdir() closedir()
#include <string.h> // strerror() && 
#include <sys/ioctl.h> //
#include <stdlib.h> //
#include <termios.h> //
#include <curses.h> //
#include <term.h> //


typedef enum e_token_type
{
	TOKEN_WORD,      // command or argument
	TOKEN_PIPE,      // |
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_NULL,        // for empty or errors
} t_token_type;

typedef struct s_token
{
	char            *value;         // the actual string (e.g. "ls", ">", "file.txt")
	t_token_type    type;           // type of token
	struct s_token  *next;          // pointer to next token
}   t_token;


/*---------------function for free--------------------*/
void			free_tokens(t_token *tokens, char *input);
void			print_tokens(t_token *head);

/*------------ tools for parsing ----------------*/
char    		**ft_split(char *str);
int     		ft_isspace(char c);
int     		is_operator(const char s);
char			*ft_strndup(const char *s1, size_t size);
char			*ft_strdup(const char *s1);
char			*ft_strncpy(char *dest, const char *src, size_t n);

/*--------------this function for tockens------------*/
t_token			*new_token(char *val, t_token_type type);
void			add_token(t_token **head, t_token *new);
t_token_type	get_token_type(const char *s);

/*---------------parsing parts-----------------------*/
t_token			*string_tokens(char *str);




#endif