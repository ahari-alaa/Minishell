/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahari <ahari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 04:59:25 by ahari             #+#    #+#             */
/*   Updated: 2025/04/24 16:06:40 by ahari            ###   ########.fr       */
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
	char            quote_type;  // '\"', '\'', or 0 (no quotes)
	struct s_token  *next;          // pointer to next token
}   t_token;

typedef struct s_file
{
	char			*name; // name of the file
	t_token_type	type;// file type
}	t_file;

typedef struct s_cmd
{
	char		**cmd; // command name
	t_file		*files; // files associated with the command
	int			file_count; // number of files
	struct s_cmd	*next; // pointer to next command
}	t_cmd;

/*---------------function for free--------------------*/
void			free_tokens(t_token *tokens, char *input);
void			free_cmd(t_cmd *cmd);
void			free_cmd_array(char **cmd);
void			free_files(t_file *files, int file_count);
void			free_cmd_list(t_cmd *cmd_list);
void			print_error(t_token *head, char *val);
/*-----------------Tokenizer --------------------------*/
t_token			*string_tokens(char *str);

/*-----------------for print--------------------------*/
void			print_tokens(t_token *head);
void			print_command_with_files(t_cmd *cmd);

/*---------------function for create------------------*/
t_cmd			*init_cmd(void);
t_file			*init_mfile(void);

/*---------------cmd----------------------------------*/
t_cmd			*parse_commands(t_token *tokens);
int				ft_isredirect(t_token_type type);
int				count_args(t_token *token);
t_token			*check_quoted(char *str);
t_cmd			*expand_cmd_list(t_cmd *cmd_head);
t_cmd			*unquote_cmd_list(t_cmd *cmd_head);

/*------------ tools for parsing ----------------*/
int				is_quote(char c);
int				ft_isspace(char c);
int				is_operator(const char s);
int				ft_isalpha(char c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
size_t			ft_strlen(const char *s);
void			ft_putstr_fd(char *s, int fd, char c);
char			*ft_strndup(const char *s1, size_t size);
char			*ft_strdup(const char *s1);
char			**ft_split(char *str);
char			*ft_strncpy(char *dest, const char *src, size_t n);
char			*ft_strcat(char *dest, const char *src);
char			*ft_substr(const char *s, unsigned int start, size_t len);
char			*ft_itoa(int n);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strcpy(char *dest, const char *src);

/*--------------this function for tockens------------*/
void			add_token(t_token **head, t_token *new);
t_token_type	get_token_type(const char *s);
t_token			*new_token(char *val, t_token_type type);

#endif