/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzaelouardi <hamzaelouardi@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 15:56:55 by ikaismou          #+#    #+#             */
/*   Updated: 2023/02/28 03:54:22 by hamzaelouar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_command(t_minishell *ms)
{
	char	*tmp;
	int		i;

	i = 0;
	while (ms->path_env[i])
	{
		tmp = ft_strjoin(ms->path_env[i], "/");
		ms->path_cmd = ft_strjoin(tmp, ms->input_cmd[0]);
		free(tmp);
		if (access(ms->path_cmd, X_OK) != -1)
			return (1);
		i++;
		free(ms->path_cmd);
	}
	return (0);
}

int count_pipe(t_minishell *ms)
{
	int	pipe;
	int i;

	i = 0;
	pipe = 0;
	while (ms->line[i])
	{
		if (ms->line[i] == '|')
			pipe++;
		i++;
	}
	return (pipe);
}

int	exec_one_pipe(t_minishell *ms, char **envp)
{
	int		id;

	if (!check_command(ms))
	{
		ft_free_tab(ms->parsed);
		return (error(CMD_ERR), 0);
	}
	id = fork();
	if (id == 0)
	{
		if (execve(ms->path_cmd, ms->parsed, envp) == - 1)
			error("error exec");
		exit(0);
	}
	ft_free_tab(ms->parsed);
	free(ms->path_cmd);
	wait(NULL);
	wait(NULL);
	return (1);
}

int	exec_multi_pipe(t_minishell *ms, char **envp, int nb_pipe)
{
	int	id;
	int	i;
	int	save_stdin;

	save_stdin = dup(0);
	i = 0;
	while (ms->parsed[i])
	{
		ms->input_cmd = ft_split(ms->parsed[i], ' ');
		if (!check_command(ms))
		{
			if (dup2(save_stdin, 0) == -1)
				error ("dup");
			ft_free_tab(ms->input_cmd);
			return (close(save_stdin), error(CMD_ERR), 0);
		}
		if (pipe(ms->fd) == -1)
			error("pipe");	
		id = fork();
		if (id == 0)
		{
			close(ms->fd[0]);
			if (nb_pipe != 0)
			{
				if (dup2(ms->fd[1], 1) == -1)
					error ("dup");
			}
			else
				dup(1);
			if (execve(ms->path_cmd, ms->input_cmd, envp) == - 1)
				error("error exec");
			exit(0);
		}
		if (nb_pipe != 0)
		{
			if (dup2(ms->fd[0], 0) == -1)
				error ("dup");
		}
		else
			if (dup2(save_stdin, 0) == -1)
				error ("dup");
		close(ms->fd[0]);
		close(ms->fd[1]);
		ft_free_tab(ms->input_cmd);
		free(ms->path_cmd);
		i++;
		nb_pipe--;
	}
	close(save_stdin);
	while (i >= 0)
	{
		wait(NULL);
		i--;
	}
	return (1);
}

int	exec_cmd(t_minishell *ms, char **envp)
{
	int		nb_pipe;

	nb_pipe = count_pipe(ms);
	if (nb_pipe == 0)
	{
		//explications :
		//ms->input_cmd a ete remplacé par ms->parsed 
		//et on a plus besoin de faire le split des espaces dans la fonction.
		if (!exec_one_pipe(ms, envp))
			return (ft_free_tab(ms->parsed), 0);
	}
	else
	{
		//explications :
		//ici on est obligé de split l'espace dans la fonction,
		//donc on touche a rien ! 
		//j'ai juste supprimé le tmp = split de '|' car ms->parsed est deja un split de '|'.
		if (!exec_multi_pipe(ms, envp, nb_pipe))
			return (ft_free_tab(ms->parsed), 0);
	}
	ft_free_tab(ms->parsed);
	return (1);
}