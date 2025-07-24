/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singal_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:11:57 by maskour           #+#    #+#             */
/*   Updated: 2025/07/21 18:51:56 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_herdoc(t_cmd *cmd)
{
	int	k;

	k = -1;
	while (++k < cmd->file_count)
	{
		if (cmd->files[k].type == TOKEN_HEREDOC)
			unlink(cmd->files[k].name);
	}
}

void	handle_empty_cmd(t_cmd *cmd)
{
	if (!cmd || !cmd->cmd)
	{
		ft_putstr_fd_up("minishell:", 2);
		ft_putstr_fd_up(" command not found\n", 2);
		exit(127);
	}
	if (!cmd->cmd[0])
	{
		handle_cmd_errors(NULL);
		exit(0);
	}
}

char	*resolve_cmd_path(t_cmd *cmd, char **env)
{
	char	*cmd_path;

	cmd_path = find_path(cmd->cmd[0], env);
	if (!cmd_path)
	{
		if (access(cmd->cmd[0], F_OK) == 0 && ft_check_dir(cmd->cmd[0]))
		{
			erro_handle(cmd->cmd[0], ": Permission denied\n");
			exit(126);
		}
		else if (ft_strcmp(cmd->cmd[0], "\0") != 0 && \
			opendir(cmd->cmd[0]) == NULL && \
			cmd->cmd[0][ft_strlen(cmd->cmd[0]) - 1] == '/')
		{
			erro_handle(cmd->cmd[0], ": Not a directory\n");
			exit(126);
		}
		if (ft_strstr(cmd->cmd[0], "/") != NULL)
			erro_handle(cmd->cmd[0], ": No such file or directory\n");
		else
			erro_handle(cmd->cmd[0], ": command not found\n");
		exit(127);
	}
	return (cmd_path);
}
