/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikaismou <ikaismou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 15:56:55 by ikaismou          #+#    #+#             */
/*   Updated: 2023/02/19 18:29:13 by ikaismou         ###   ########.fr       */
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




/*int	exec_cmd(t_minishell *ms, char **envp)
{
	char	**tmp;
	int		id;

	(void)(envp);
	tmp = ft_split(ms->line, '\n');
	ms->input_cmd = ft_split(tmp[0], ' ');
	if (!check_command(ms))
	{
		ms->prompt = ERR_PROMPT;
		return (error(CMD_ERR), 0);
	}
	id = fork();
	if (id == 0)
	{
		if (execve(ms->path_cmd, ms->input_cmd, envp) == - 1)
			return (exit(0), 0);
		if (write(1, ms->prompt, ft_strlen(ms->prompt)) == -1)
			return (0);
		exit(0);
	}
	ms->prompt = "👨‍💻 Minishell> ";
	wait(NULL);
	return (1);
}*/


/////////////////////////////////////VERSION PIPE NON FINI//////////////////////////////////////////

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

int	exec_one_pipe(t_minishell *ms, char **envp, char **tmp)
{
	int		id;
	int		i;
	int		j;

	i = 0;
	while (tmp[i])
	{
		j = 0;
		ms->input_cmd = ft_split(tmp[i], ' ');
		while (ms->input_cmd[j])
		{
			ft_printf("%s\n", ms->input_cmd[j]);
			j++;
		}
		if (!check_command(ms))
		{
			ft_free_tab(ms->input_cmd);
			//free(ms->path_cmd);
			// ms->prompt = ERR_PROMPT;
			return (error(CMD_ERR), 0);
		}
		id = fork();
		if (id == 0)
		{
			if (execve(ms->path_cmd, ms->input_cmd, envp) == - 1)
				return (exit(0), 0);
			exit(0);
		}
		ft_free_tab(ms->input_cmd);
		free(ms->path_cmd);
		i++;
	}
	while (i > 0)
	{
		wait(NULL);
		i--;
	}
	return (1);
}

int	exec_multi_pipe(t_minishell *ms, char **envp, char **tmp2, int nb_pipe)
{
	int		id;
	int		i;
	// int		j;

	i = 0;
	while (tmp2[i])
	{
		//j = 0;
		ms->input_cmd = ft_split(tmp2[i], ' ');
		// while (ms->input_cmd[j])
		// {
		// 	ft_printf("%s\n", ms->input_cmd[j]);
		// 	j++;
		// }
		if (!check_command(ms))
		{
			ms->prompt = ERR_PROMPT;
			return (error(CMD_ERR), 0);
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
					return (exit(0), 0);
			}
			if (execve(ms->path_cmd, ms->input_cmd, envp) == - 1)
				return (exit(0), 0);
			exit(0);
		}
		if (nb_pipe != 0)
		{
			if (dup2(ms->fd[0], 0) == -1)
				return (exit(0), 0);
		}
		close(ms->fd[0]);
		close(ms->fd[1]);
		ft_free_tab(ms->input_cmd);
		free(ms->path_cmd);
		i++;
		nb_pipe--;
	}
	ft_free_tab(tmp2);
	while (i > 0)
	{
		wait(NULL);
		i--;
	}
	return (1);
}

int	exec_cmd(t_minishell *ms, char **envp)
{
	char	**tmp;
	char	**tmp2;
	int		nb_pipe;

	(void)(envp);
	tmp = ft_split(ms->line, '\n');
	nb_pipe = count_pipe(ms);
	if (nb_pipe == 0)
	{
		if (!exec_one_pipe(ms, envp, tmp))
			return (0);
	}
	else
	{
		tmp2 = ft_split(tmp[0], '|');
		if (!exec_multi_pipe(ms, envp, tmp2, nb_pipe))
			return (0);
	}
	ft_free_tab(tmp);
	return (1);
}