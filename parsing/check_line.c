/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzaelouardi <hamzaelouardi@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 01:26:18 by hamzaelouar       #+#    #+#             */
/*   Updated: 2023/02/28 04:25:56 by hamzaelouar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*quote_2(char *line, char *str)
{
	int		i;
	int		i2;
	char	c;

	str = ft_calloc(sizeof(char), (ft_strlen(line) - 2 + 1));
	i = 0;
	i2 = 0;
	while (line[i2])
	{
		if (line[i2] && ft_strchr("\'\"", line[i2]))
		{
			c = line[i2++];
			while (line[i2] && line[i2] != c)
				str[i++] = line[i2++];
			i2++;
		}
		else if (line[i2])
			str[i++] = line[i2++];
	}
	str[i] = 0;
	return (str);
}

char	*quote(char *line)
{
	char	*str;

	str = NULL;
	if (!line)
		return (0);
	if (ft_strchr(line, '\'') || ft_strchr(line, '\"'))
		str = quote_2(line, str);
	else
		return (line);
	return (str);
}

static void del_char(t_minishell *ms)
{
    int size;
    int i;
	int j;
    
    i = 0;
    size = 0;
    while (ms->line[i])
    {
        if (ms->line[i] != 92 && ms->line[i] != ';')
            size++;
		i++;
    }
    ms->new_line = malloc(sizeof(char) * (size + 1));
    if (!ms->new_line)
    {
        free(ms->new_line);
        return ;
    }
    i = 0;
    j = 0;
    while (ms->line[i])
    {
        if (ms->line[i] != 92 && ms->line[i] != ';')
		{
            ms->new_line[j] = ms->line[i];
			j++;
		}
		i++;

    }
    ms->new_line[j] = 0;
}

void new(t_minishell *ms, char **space)
{
    (void)(ms);
    int i;
    char **quot;

    i = 0;
    while (space[i])
        i++;
    quot = malloc(sizeof(char *) * i + 1);
	ms->parsed = malloc(sizeof(char *) * i + 1);
    i = 0;
    while (space[i])
    {
        quot[i] = quote(space[i]);
        i++;
    }
    quot[i] = 0;
    i = 0;
    while (quot[i])
    {
        ms->parsed[i] = ft_strdup(quot[i]);
        i++;
    }
    ms->parsed[i] = 0;
    ft_free_tab(quot);
}

void    ft_pipe(t_minishell *ms, char **space)
{
    char	**pipe;
	int		i;
    int j = 0;
	
    pipe = ft_split(ms->new_line, '|');
	while (pipe[j])
		j++;
	ms->joined = malloc(sizeof(char *) * (j + 1));
	j = 0;
    while (pipe[j])
    {
        space = ft_split(pipe[j], ' ');
        new(ms, space);
		i = 0;
		while (ms->parsed[i])
		{
			ms->joined[j] = ft_strjoin_gnl(ms->joined[j], ms->parsed[i]);
			ms->joined[j] = ft_strjoin_gnl(ms->joined[j], " ");
			i++;
		}
		ft_free_tab(ms->parsed);
        j++;
    }
	ft_free_tab(pipe);
}

void check_new_line(t_minishell *ms)
{
    int i;
    char **space;
    
	space = NULL;
    i = 0;
	//explications :
	//ms->new_line c'est le ms->uchar que t'as cree.
	//dans un cas ms->new_line est une modification ms->line sans les ';' et '\'
	// et dans l'autre cas c'est juste une copie de ms->line
	// comme ça on utilisera que ms->new_line pour TOUT à partir d'ici,
	// sauf pour add_history(ms->line) qui mettra les ';' et '\' dans l'history.
    if (ft_strchr(ms->line, 92) || ft_strchr(ms->line, ';'))
        del_char(ms);
    else
        ms->new_line = ft_strdup(ms->line);
	//explications :
	//dans le cas ou ya pas de '|', on obtientra un char ** ms->parsed 
	//avec les commandes parsées et pretes à etre exec.
	//Dans le cas des '|', obligé de split pipe, de creer un char **ms->joined
	//qui sert à remettre les espaces apres les avoir split,
	// et au final ms->parsed devient la copie de ms->joined
	// sans ça on aurait du manipuler des char ***
	//ça simplifie de ouf l'exec pour les pipe

	//pour les leaks j'en ai que avec des '|' regarde avec valgrind.
    if (ft_strchr(ms->new_line, '|'))
	{
        ft_pipe(ms, space);
		while (ms->joined[i])
			i++;
		ms->parsed = malloc(sizeof(char *) * (i + 1));
		i = 0;
		while(ms->joined[i])
		{
			ms->parsed[i] = ft_strdup(ms->joined[i]);
			i++;
		}
		ms->joined[i] = 0;
		ft_free_tab(ms->joined);
	}
    else
    {
        space = ft_split(ms->new_line, ' ');
        new(ms, space);
    }
	//explications de la suite :
	//juste le printf pour verifier mdrr. 
	i = 0;
	while(ms->parsed[i])
	{
		ft_printf("fin parsed :%s\n", ms->parsed[i]);
		i++;
	}  
}