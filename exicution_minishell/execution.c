/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/04/24 16:15:18 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
char	**splite_cmd(char *str)
{
	char	**splt;

	splt = ft_split_up(str, ' ');
	if (!splt)
		return (NULL);
	return (splt);
}
static void handle_cmd_errors(char *cmd_path)
{
    char *s = cmd_path;
    if(cmd_path)
        free(cmd_path);
    printf("%s:,%s\n",s,"cmmand not found");
    exit(1);
}
static void cmd_process(t_cmd *cmd, char **env)
{
    char *cmd_path;
    // char **cmd_arg;
    if (!cmd || !cmd->cmd || !cmd->cmd[0])
        handle_cmd_errors(NULL);
    if (redirections(cmd) != 0)// that's to handel the rederections 
        exit(1);
    // cmd_arg = split_cmd(&cmd);
    cmd_path = find_path(cmd->cmd[0], env);
    // if (!cmd_path)
    //     handle_cmd_error(NULL, NULL);
    if(execve(cmd_path,cmd->cmd, env) == -1)
        handle_cmd_errors(cmd_path);
}
static void execute_single_command(t_cmd **cmd, char **envp)
{
    pid_t id;
    int status;
    id = fork();
    // printf("single_command\n");
    if (id == 0)
        cmd_process(*cmd,envp);
    else if (id > 0)
        waitpid(id, &status, 0);
    else
        {
            perror("fork fild");
            return ;
        }
}

int exicut(t_cmd **cmd, char **env)
{
    int cmd_count = 0;
    if (!cmd || !*cmd)
        return (1);
    
    while (cmd[cmd_count])
        cmd_count++;
    if (cmd_count == 1)
        execute_single_command(cmd, env);
    // else
    //     execute_pipeline(cmd, env);
    return (0);
}