/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 04:59:25 by ahari             #+#    #+#             */
/*   Updated: 2025/07/21 18:42:25 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <signal.h>
#include <wait.h>
# include <limits.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <dirent.h>
# include <stdlib.h>
# include <term.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_NULL,
}	t_token_type;

typedef struct s_shell
{
	int	exit_status;
}	t_shell;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				was_quoted;
	struct s_token	*next;
}	t_token;

typedef struct s_file
{
	char			*name;
	t_token_type	type;
	int				check_expand;
}	t_file;

typedef struct s_cmd
{
	char			**cmd;
	t_file			*files;
	int				file_count;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*data_env;
	struct s_env	*next;
}	t_env;

typedef struct s_env_list
{
	char		**env_table;
	t_shell		*exit_status;
}	t_env_list;

typedef struct s_exec_ctx
{
	int			i;
	int			cmd_index;
	int			cmd_count;
	int			*prev_pipe;
	int			pipes[2];
	pid_t		*last_pid;
	t_shell		*shell_ctx;
}	t_exec_ctx;

typedef struct s_export_arg
{
	t_env	*current;
	char	*tmp;
}	t_export_arg;

typedef struct s_get_nam_arg
{
	char	*filename;
	int		count;
	char	*numbered_name;
	char	*full_path;
}	t_get_nam_arg;

typedef struct s_herdoc_arg
{
	char	*line;
	int		fd;
	int		h;
	int		f;
	int		hd_status;
}	t_herdoc_arg;

/*---------------function for free--------------------*/
void			free_tokens(t_token *tokens, char *input);
void			free_char_array(char **array);
void			free_cmd(t_cmd *cmd);
void			free_cmd_array(char **cmd);
void			free_files(t_file *files, int file_count);
void			free_cmd_args(char **args, int count);
void			free_cmd_list(t_cmd *cmd_list);
void			print_error(t_token *head, char *val, t_shell *shell_ctx);
void			print_syntax_error(t_token *current);
void			free_array(char **array);

/*-----------------Tokenizer --------------------------*/
t_token			*string_tokens(char *str, t_shell *shell_ctx);
t_token			*find_previous_token(t_token *head, t_token *target);

/*-----------------parsing --------------------------*/
int				is_export_assignment(t_token *head, t_token *current);
char			*process_quoted_value(char *val, t_token *head,
					t_env_list *env);

/*---------------function for create------------------*/
t_cmd			*init_cmd(void);
t_file			*init_mfile(void);

/*---------------parsing_herdoc-----------------------*/
char			*safe_strcat_heredoc(char *dest, char *src);
char			*process_quoted_heredoc(char *val, int *i);

/*---------------main tools--------------------------*/
int				handle_here_docs(t_cmd *commands, t_shell *shell_ctx,
					t_token *tokens, char **env_table);
int				handle_input(char *input, char **env_table, t_token **tokens,
					t_shell *shell_ctx);
int				handle_commands(t_token *tokens, t_shell *shell_ctx,
					char **env_table, t_cmd **commands);

/*---------------parsing_v2--------------------------*/
char			*realloc_result(char *result, size_t *capacity, size_t new_len);
char			*append_to_result(char *result, char *tmp, size_t *capacity);
int				count_commands(t_token *tokens);
int				parse_arguments(t_cmd *cmd, t_token *tokens);
int				process_file_redirection(t_cmd *cmd, t_token *current);
int				validate_redirection_token(t_token *current);
t_token			*create_and_add_token(char *val, t_token **head);

/*---------------cmd----------------------------------*/
t_cmd			*parse_commands(t_token *tokens, t_shell *shell_ctx);
int				ft_isredirect(t_token_type type);
int				count_args(t_token *token);
t_token			*check_quoted(char *str, t_shell *shell_ctx, char **env_table);
char			**convert(t_env *env_list);
int				count_herdoc(t_cmd *cmd);
int				process_token(t_token *current, t_token **head,
					t_env_list *env_list);
char			*herdoc_parsing(char *val);
char			*remove_char(const char *str, char to_remove);
char			*remove_dollar_before_quotes(char *str, char **temp);
int				check_syntax_errors(char *str, int i, t_shell *shell_ctx);
int				validate_syntax(char *str, t_shell *shell_ctx);
char			*extract_word_token(char *str, int start, int end);
int				count_redirections(t_token *start);
int				process_quote_parsing(char *str, int *i, char *quote_type,
					int *in_quotes);

/*------------ tools for parsing ----------------*/
int				is_quote(char c);
int				is_char(char c);
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);
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
int				ft_strspace(char *str);
void			ft_putchar_fd(char c, int fd);
char			*ft_exit_status(t_shell *exit_stat);
char			*ft_delete_spaces(char *str);
int				ft_strspaces(char *str);
char			*ft_strcpy(char *dest, const char *src);
char			*found_env(char *cmd, char **env, t_shell *shell_ctx);
int				has_quotes(char *str);

/*---------------this function for expand------------*/
char			*handle_special_var(char *cmd, int pos, t_shell *exited);
char			*get_env(char **env, char *found_env);
char			*handle_env_var(char *cmd, int pos, char **env);
char			*replace_double_dollar(char *cmd);
int				should_expand_var(char *cmd, int pos);

/*--------------this function for tokens------------*/
void			add_token(t_token **head, t_token *new);
t_token_type	get_token_type(const char *s);
t_token			*new_token(char *val, t_token_type type);

/*---------------execution_util-----------------------*/
// char	*ft_itoa(int n);
char			**ft_split_up(char const *s, char c);
char			*ft_strchr(const char *src, int c);
char			*ft_strdup(const char *s1);
char			*ft_strjoin(char const *s1, char const *s2);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t			ft_strlcpy(char *dest, const char *src, size_t dstsize);
size_t			ft_strlen(const char *str);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
void			ft_putstr_fd_up(char *s, int fd);
int				ft_atoi(const char *str);
int				ft_isalpha_up(int c);
char			*ft_strstr(const char *haystack, const char *needle);
int				ft_check_dir(char *path);
char			*ft_strtrim(char const *s1, char const *set);
int				is_spaces(char *s);

/*---------------execution_util-----------------------*/
int				exicut(t_cmd **cmd, t_env **env_list, t_shell *shell_ctx);
int				redirections(t_cmd *cmd, int last_heredoc_index);
char			*find_path(char *cmd, char **env);

/*---------------------builtins-----------------------*/
t_env			*execut_bultin(t_cmd **cmd, t_env *env_list, t_shell *shell,
					int i);
int				is_builtin(char *command);
t_env			*ft_cd(t_cmd **cmd, t_env *data_env, t_shell *shell_ctx);
void			update(t_env *data_env, char *oldpwd_update, char *pwd_update);
int				access_path(char *path, t_shell *shell_ctx);
void			check_getcwd(char *oldpwd_update, t_cmd *cmd_path,
					t_shell *shell_ctx);
char			*validate_path(t_cmd *cmd_path, t_env *data_env,
					t_shell *shell_ctx);
void			add_oldpwd(t_env *data_env);
void			ft_echo(t_cmd **cmd, t_shell *shell_ctx);
void			ft_env(t_env *env_list, t_shell *shell_ctx);
void			ft_exit(t_cmd **cmd, t_shell *shell, int j);
void			ft_pwd(t_shell *shell_ctx, t_env *env_list);
t_env			*ft_unset(t_cmd **cmd, t_env *env, t_shell *shell_ctx);
void			ft_export(t_cmd **cmd, t_env **envp, t_shell *shell);
void			sort_and_display_env(t_env **envp);
void			add_env_export(t_env **env, char *key, char *value);
void			update_or_add_env(t_env **env, char *key, char *value);
int				update_env_value(t_env *current, char *key, char *value);
int				is_valid_key(char *key);
int				is_exist(t_env *env, char *key);
void			extra_key_value(char *input, char **key, char **value);
void			append_env_node(t_env **env, t_env *new_node);
int				update_exist_env(t_env *env, char *key, char *value);

/*---------------builtins-utils-----------------------*/
char			*search_env(t_env *env, const char *key);
t_env			*new_env(char *data_env);
void			add_env(t_env **env_list, t_env *new_node);
t_env			*file_inv(char **env);
void			free_env_list(t_env *env_list);

/*---------------signals-----------------------*/
void			handler_sig(int signal);
void			ignore_sigint(void);
void			restore_sigint(void);

/*---------------------error utils----------------------------------*/
int				put_error_ambig(char *file_name);
int				handle_err_file(t_file *file);
void			erro_handle(char *cmd, char *err_message);
void			handle_cmd_errors(char *cmd_path);

void			free_split(char **str);
void			free_env(char **env);
void			remove_env_key(t_env **env, const char *key);
char			*get_rundem_name(char *file_name);

int				find_last_heredoc_index(t_cmd *cmd);
void			cleanup_heredoc_files(t_cmd **cmds, int cmd_count);
int				process_all_heredocs(t_cmd **cmds, int cmd_count,
					char **env, t_shell *shell_ctx);
void			setup_pipes_in_child(int i, int cmd_count, int prev_pipe,
					int pipes[2]);
void			update_exit_status(int wstatus, t_shell *shell_ctx);
int				function_herdoc(t_file *file, char **env, t_shell *shell_ctx);
void			execute_single_command(t_cmd **cmd, t_env **env_list,
					char **envp, t_shell *shell_ctx);
void			execute_pipeline(t_cmd **cmds, char **env, t_env *env_list,
					t_exec_ctx *exec_ctx);
int				open_file(char *file, int mode);
int				cleanup_stdio(int original_stdin, int original_stdout);
int				is_ambiguous_redirect(const char *filename);
int				apply_input_redirection(int last_in_fd, int orig_stdin,
					int orig_stdout);
int				handle_ambiguous_redirect(t_file *file, int original_stdin,
					int original_stdout);
int				apply_output_redirection(int last_out_fd, int orig_stdin,
					int orig_stdout);
void			setup_terminal_and_signals(struct termios *original_termios);
void			restore_terminal_and_signals(struct termios *original_termios);
char			*resolve_cmd_path(t_cmd *cmd, char **env);
void			handle_empty_cmd(t_cmd *cmd);
void			cleanup_herdoc(t_cmd *cmd);
void			execute_child(t_cmd **cmds, char **env, t_env *env_list,
					t_exec_ctx *ctx);
void			handle_parent_process(int i, int *prev_pipe, int pipes[2],
					int cmd_count);
int				first_sort_env(t_cmd *cmd, t_env **envp, t_shell *shell);

#endif