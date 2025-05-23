/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamza <hamza@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:49:02 by hel-ouar          #+#    #+#             */
/*   Updated: 2023/05/14 10:19:13 by hamza            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	open_infile_simple(t_minishell *ms, int i, int j)
{
	while (ms->parsed[i] && ms->parsed[i][0] == '<')
	{
		if (ms->parsed[i][1] == '<')
			j++;
		i++;
	}
	i--;
	if (ms->parsed[i][1] == '<')
	{
		ms->parsed[i] = quote(ms->parsed[i]);
		ms->infile = open(ms->f_name[j - 1], O_RDONLY);
		if (ms->infile < 0)
			error_exit(ms->parsed[i] + 2, ": Permission denied\n", 1);
	}
	if (ms->parsed[i][1] != '<')
	{
		ms->parsed[i] = quote(ms->parsed[i]);
		ms->infile = open(ms->parsed[i] + 1, O_RDONLY);
		if (ms->infile < 0)
			error_exit(ms->parsed[i] + 1, ": Permission denied\n", 1);
	}
	return (i + 1);
}

int	open_outfile_simple(t_minishell *ms, int i)
{
	while (ms->parsed[i] && ms->parsed[i][0] == '>')
	{
		if (ms->parsed[i][0] == '>')
		{
			ms->outfile_exist = 1;
			if (ms->parsed[i][1] == '>')
			{
				ms->parsed[i] = quote(ms->parsed[i]);
				ms->outfile = open(ms->parsed[i] + 2, \
					O_CREAT | O_RDWR | O_APPEND, 0644);
				if (ms->outfile < 0)
					error_exit(ms->parsed[i] + 2, ": Permission denied\n", 1);
			}
			else
			{
				ms->parsed[i] = quote(ms->parsed[i]);
				ms->outfile = open(ms->parsed[i] + 1, \
					O_CREAT | O_RDWR | O_TRUNC, 0644);
				if (ms->outfile < 0)
					error_exit(ms->parsed[i] + 1, ": Permission denied\n", 1);
			}
		}
		i++;
	}
	return (i);
}

char	**check_redir_simple(t_minishell *ms)
{
	int		i;
	int		size;

	size = 0;
	i = 0;
	access_file2(ms);
	while (ms->parsed[size])
		size++;
	if (ms->parsed[0][0] == '<')
	{
		i = open_infile_simple(ms, 0, 0);
		if (dup2(ms->infile, 0) == -1)
			error ("dup");
	}
	if (ms->parsed[i][0] == '>')
	{
		i = open_outfile_simple(ms, i);
		if (dup2(ms->outfile, 1) == -1)
			error ("dup6");
	}
	rm_quote_last(ms->parsed);
	i--;
	return (ft_realloc_from_i(ms->parsed, size, i));
}
