/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikaismou <ikaismou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:07:49 by ikaismou          #+#    #+#             */
/*   Updated: 2023/05/04 17:19:42 by ikaismou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**open_files(t_minishell *ms, char **tab)
{
	int i;
	char	**realloc;
	int		j;
	char	*tab2;
	j = 0;
	i = 0;
	access_file(tab, ms);
	if (tab[i] && tab[i][0] == '<')
	{
		while (tab[i] && tab[i][0] == '<')
			i++;
		i--;
		if (tab[i][1] == '<')
		{
			tab[i] = quote(tab[i]);
			tab2 = ft_strjoin(".", tab[i] + 2);
			ms->infile = open(tab2, O_RDONLY);
			if (ms->infile < 0)
			{
				ms->infile_str = tab[i] + 2;
			}
		}
		if (tab[i][1] != '<')
		{
			tab[i] = quote(tab[i]);
			ms->infile = open(tab[i] + 1, O_RDONLY);
			if (ms->infile < 0)
			{
				ms->infile_str = tab[i] + 1;
			}
		}
		i++;
	}
	if (tab[i] && tab[i][0] == '>')
	{
										
		while (tab[i] && tab[i][0] == '>')
		{
			if (tab[i][0] == '>')
			{
				ms->outfile_exist = 1;
				if (tab[i][1] == '>')
				{
					tab[i] = quote(tab[i]);
					ms->outfile = open(tab[i] + 2, O_CREAT | O_RDWR | O_APPEND, 0644);
					if (ms->outfile < 0)
					{
						ms->infile_str = tab[i] + 2;
						break ;
					}
				}
				else
				{
					tab[i] = quote(tab[i]);
					ms->outfile = open(tab[i] + 1, O_CREAT | O_RDWR | O_TRUNC, 0644);
					if (ms->outfile < 0)
					{
						ms->infile_str = tab[i] + 1;
						break ;
					}
				}
			}
			tab[i] = quote(tab[i]);
			i++;
		}
	}
	if (tab[i] && (tab[i][0] == '>' || tab[i][0] == '<'))
	{
		i = 0;
		while (tab[i] && (tab[i][0] == '>' || tab[i][0] == '<'))
			i++;
	}
	i--;
	realloc = ft_gc_malloc(sizeof(char *) * (ft_strlen_dtab(tab) - i));
	i++;
	while (tab[i])
	{
		realloc[j] = ft_strdup(tab[i]);
		j++;
		i++;
	}
	realloc[j] = 0;
	return (realloc);
}


char	**check_redir2(t_minishell *ms)
{
	int i;
	char	**realloc;
	int		size;
	int		j;
	char	*tab2;

	j = 0;
	size = 0;
	i = 0;
	access_file2(ms->parsed);
	while (ms->parsed[size])
		size++;
	if (ms->parsed[i][0] == '<')
	{
		
		while (ms->parsed[i] && ms->parsed[i][0] == '<')
			i++;
		i--;
		if (ms->parsed[i][1] == '<')
		{
			tab2 = ft_strjoin(".", ms->parsed[i] + 2);
			ms->infile = open(tab2, O_RDONLY);
			if (ms->infile < 0)
			{
				ft_dprintf(""RED"bash: %s: Permission denied\n"WHITE"", ms->parsed[i] + 2);
				exit (1);
			}
		}
		if (ms->parsed[i][1] != '<')
		{
			ms->infile = open(ms->parsed[i] + 1, O_RDONLY);
			if (ms->infile < 0)
			{
				ft_dprintf(""RED"bash: %s: Permission denied\n"WHITE"", ms->parsed[i] + 1);
				exit (1);
			}
		}
		if (dup2(ms->infile, 0) == -1)
			error ("dup");
		i++;
	}
	if (ms->parsed[i][0] == '>')
	{
		while (ms->parsed[i] && ms->parsed[i][0] == '>')
		{
			if (ms->parsed[i][0] == '>')
			{
				ms->outfile_exist = 1;
				if (ms->parsed[i][1] == '>')
				{
					ms->outfile = open(ms->parsed[i] + 2, O_CREAT | O_RDWR | O_APPEND, 0644);
					if (ms->outfile < 0)
					{
						ft_dprintf(""RED"bash: %s: Permission denied\n"WHITE"", ms->parsed[i] + 2);
						exit (1);
					}
				}
				else
				{
					ms->outfile = open(ms->parsed[i] + 1, O_CREAT | O_RDWR | O_TRUNC, 0644);
					if (ms->outfile < 0)
					{
						ft_dprintf(""RED"bash: %s: Permission denied\n"WHITE"", ms->parsed[i] + 1);
						exit (1);
					}
				}
			}
			i++;
		}
		if (dup2(ms->outfile, 1) == -1)
			error ("dup6");
	}
	i--;
	realloc = ft_gc_malloc(sizeof(char *) * (size - i));
	i++;
	while (ms->parsed[i])
	{
		realloc[j] = ft_strdup(ms->parsed[i]);
		j++;
		i++;
	}
	realloc[j] = 0;
	return (realloc);
}