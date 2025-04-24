/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/04/24 20:48:57 by maskour          ###   ########.fr       */
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
static void	execute_pipeline(t_cmd **cmds, int cmd_count, char **env, int infile, int outfile)
{

	int		i;
	int		pipfd[2];
	pid_t	pid;
	int		prev_pipe_in = -1;

	i = -1;
	while (++i < cmd_count)
	{
		if (i < cmd_count - 1 && pipe(pipfd) == -1)
			{
				perror("pipe failed");
				exit(1);
			}
		pid = fork();
		if (pid == -1)
			{
			perror("fork failed");	
			exit(1);
			}
		if (pid == 0)
		{
			if (i == 0) // First command: read from infile
				dup2(infile, 0);
			else // Middle commands: read from previous pipe
				dup2(prev_pipe_in, 0);
			if (i == cmd_count - 1) // Last command: write to outfile
				dup2(outfile, 1);
			else // Middle commands: write to next pipe
				dup2(pipfd[1], 1);
			close(pipfd[0]);
			close(pipfd[1]);
			cmd_process(cmds[i], env);
		}
		if (prev_pipe_in != -1)
			close(prev_pipe_in);
		if (i < cmd_count - 1)
			prev_pipe_in = pipfd[0];
		close(pipfd[1]);
	}
	close(infile);
	close(outfile);
	while (wait(NULL) != -1)
		;
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
    else
        execute_pipeline(cmd, cmd_count,env,0 ,1);
    return (0);
}