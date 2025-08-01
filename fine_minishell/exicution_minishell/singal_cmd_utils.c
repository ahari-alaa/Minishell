/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singal_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:11:57 by maskour           #+#    #+#             */
/*   Updated: 2025/07/31 17:54:38 by maskour          ###   ########.fr       */
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

static char	*remove_trailing_slash(const char *path)
{
	size_t	len;

	if (!path)
		return (NULL);
	len = ft_strlen(path);
	if (len > 0 && path[len - 1] == '/')
		return (ft_substr(path, 0, len - 1));
	return (ft_strdup(path));
}

char	*resolve_cmd_path(t_cmd *cmd, char **env)
{
	char	*cmd_path;
	char	*trimmed;
	int		has_slash;

	trimmed = remove_trailing_slash(cmd->cmd[0]);
	has_slash = (ft_strchr(cmd->cmd[0], '/') != NULL);
	cmd_path = find_path(cmd->cmd[0], env);
	if (cmd_path)
		return (free(trimmed), cmd_path);
	if (access(cmd->cmd[0], F_OK) == 0 && has_slash)
		return (erro_handle(cmd->cmd[0], ": Permission denied\n"),
			free(trimmed), exit(126), NULL);
	if (has_slash)
	{
		if (access(trimmed, F_OK) == 0)
			return (erro_handle(trimmed, ": Not a directory\n"),
				free(trimmed), exit(126), NULL);
		return (erro_handle(trimmed, ": No such file or directory\n"),
			free(trimmed), exit(127), NULL);
	}
	erro_handle(cmd->cmd[0], ": command not found\n");
	return (free(trimmed), exit(127), NULL);
}
