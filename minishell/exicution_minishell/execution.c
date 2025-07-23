/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:01:55 by maskour           #+#    #+#             */
/*   Updated: 2025/07/23 19:02:49 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_cmd(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static t_cmd	**convert_lit_arr(t_cmd *cmd, int cmd_count)
{
	int		i;
	t_cmd	*current;
	t_cmd	**cmd_arr;

	cmd_arr = malloc(sizeof(t_cmd *) * (cmd_count + 1));
	current = cmd;
	if (!cmd_arr)
		return (NULL);
	i = -1;
	while (++i < cmd_count)
	{
		cmd_arr[i] = current;
		current = current->next;
	}
	cmd_arr[cmd_count] = NULL;
	return (cmd_arr);
}

static void	execute_single(t_cmd **cmd, char **env,
				t_env **env_list, t_shell *shell_ctx)
{
	if (is_builtin((*cmd)->cmd[0]) && (*cmd)->file_count == 0)
	{
		*env_list = execut_bultin(cmd,*env_list, shell_ctx, 1);
	}
	execute_single_command(cmd, env_list, env, shell_ctx);
}

int	exicut(t_cmd **cmd, t_env **env_list, t_shell *shell_ctx)
{
	int			cmd_count;
	t_cmd		**cmd_arr;
	char		**env;
	t_exec_ctx	exec_ctx;

	if (!cmd || !*cmd || !env_list)
		return (1);
	env = convert(*env_list);
	if (!env)
		return (1);
	cmd_count = count_cmd(*cmd);
	exec_ctx.cmd_count = cmd_count;
	exec_ctx.shell_ctx = shell_ctx;
	if (cmd_count == 1)
		execute_single(cmd, env, env_list, shell_ctx);
	else
	{
		cmd_arr = convert_lit_arr(*cmd, cmd_count);
		if (!cmd_arr)
			return (free_env(env), 1);
		(execute_pipeline(cmd_arr, env, *env_list, &exec_ctx), free(cmd_arr));
	}
	free_env(env);
	return (0);
}
