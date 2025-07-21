/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:28:52 by maskour           #+#    #+#             */
/*   Updated: 2025/07/18 22:37:04 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*build_tmp_path(char *prefix, char *file_name)
{
	char	*tmp;

	tmp = ft_strjoin(prefix, file_name);
	return (tmp);
}

static int	try_create_file(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
	if (fd != -1)
		close(fd);
	return (fd);
}

static char	*build_numbered_name(char *file_name, int count)
{
	char	*count_str;
	char	*tmp;
	char	*numbered;

	count_str = ft_itoa(count);
	if (!count_str)
		return (NULL);
	tmp = ft_strjoin(file_name, "_");
	if (!tmp)
	{
		free(count_str);
		return (NULL);
	}
	numbered = ft_strjoin(tmp, count_str);
	free(tmp);
	free(count_str);
	return (numbered);
}

static char	*build_full_numbered_path(char *numbered_name)
{
	char	*tmp;

	tmp = ft_strjoin("/tmp/minishell_", numbered_name);
	return (tmp);
}

char	*get_rundem_name(char *file_name)
{
	t_get_nam_arg	ctx;

	ctx.filename = build_tmp_path("/tmp/minishell_", file_name);
	if (!ctx.filename)
		return (NULL);
	if (try_create_file(ctx.filename) != -1)
		return (ctx.filename);
	free(ctx.filename);
	ctx.count = -1;
	while (++(ctx.count) < 1000)
	{
		ctx.numbered_name = build_numbered_name(file_name, ctx.count);
		if (!ctx.numbered_name)
			return (NULL);
		ctx.full_path = build_full_numbered_path(ctx.numbered_name);
		free(ctx.numbered_name);
		if (!ctx.full_path)
			return (NULL);
		if (try_create_file(ctx.full_path) != -1)
			return (ctx.full_path);
		free(ctx.full_path);
	}
	return (NULL);
}
