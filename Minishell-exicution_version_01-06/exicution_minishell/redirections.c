/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:02:07 by maskour           #+#    #+#             */
/*   Updated: 2025/05/26 22:30:06 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
static int open_file(char *file, int mode)
{
    int fd = 0;
    if (mode == 0)
        fd = open(file, O_RDONLY);
    else if (mode == 1)
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (mode == 2)
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    return (fd);
}

// static void *get_rundem_name(char *file_name)
// {
//     char *base = "tmp";
//     char *filename;
//     int fd;
//     char *count_str;
//     char *tmp;

//     // First attempt with base name
//     filename = ft_strdup(file_name);
//     if (!filename)
//         return (NULL);

//     fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
//     if (fd != -1)
//     {
//         close(fd);
//         return (filename);
//     }
//     free(filename);

//     // Try with numbered suffixes
//     int count = 0;
//     while (count < 1000)
//     {
//         count_str = ft_itoa(count);
//         if (!count_str)
//             return (NULL);

//         filename = ft_strjoin(file_name, "_");
//         if (!filename)
//         {
//             free(count_str);
//             return (NULL);
//         }

//         tmp = ft_strjoin(filename, count_str);
//         free(filename);
//         free(count_str);
        
//         if (!tmp)
//             return (NULL);

//         filename = tmp; // take ownership
//         // Don't free(tmp)!

//         fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
//         if (fd != -1)
//         {
//             close(fd);
//             return (filename); // caller must free this
//         }

//         free(filename);
//         count++;
//     }
//     return (NULL);
// }

static int function_herdoc(t_file *file)
{
    char *filename = file->name;   // Don't free filename unless you strdup'ed it here
    if (!filename) {
        perror("minishell: cannot create temporary file");
        return (1);
    }

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("minishell: open");
        return (1);
    }

    char *line;
    printf("delimiter is : %s\n", file->name);
    while (1) 
    {
        line = readline("> ");
        if (!line)
            break;
        if (!ft_strcmp(line, file->name)) {
            free(line);
            break ;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        ft_putstr_fd_up("minishell: dup2", 2);
        unlink(filename);
        return (1);
    }
    unlink(filename);
    dup2(fd, 0);
    close(fd);

    signal(SIGINT, handler_sig);
    signal(SIGQUIT, handler_sig);
    return (0);
}

int redirections(t_cmd *cmd)
{
    if (!cmd || cmd->file_count <= 0)
        return 0; // Nothing to do, considered success

    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    if (original_stdin == -1 || original_stdout == -1) {
        perror("minishell: dup");
        return 1;
    }
    int i = -1;
    while (++i < cmd->file_count) 
    {
        t_file *file = &cmd->files[i];
        int fd = -1;

        if (file->type == TOKEN_REDIRECT_IN) {
            fd = open_file(file->name, 0);
            if (fd == -1 || dup2(fd, STDIN_FILENO) == -1) {
                if (fd != -1) close(fd);
                perror("minishell");
                goto cleanup;
            }
            close(fd);
        }
        else if (file->type == TOKEN_REDIRECT_OUT) {
            fd = open_file(file->name, 1);
            if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1) {
                if (fd != -1) close(fd);
                perror("minishell");
                goto cleanup;
            }
            close(fd);
        }
        else if (file->type == TOKEN_APPEND) {
            fd = open_file(file->name, 2);
            if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1) {
                if (fd != -1) close(fd);
                perror("minishell");
                goto cleanup;
            }
            close(fd);
        }
        else if (file->type == TOKEN_HEREDOC) {
            if (function_herdoc(file)) {
                goto cleanup;
            }
        }
    }

    close(original_stdin);
    close(original_stdout);
    return 0; // Success

cleanup:
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
    return 1; // Error
}
