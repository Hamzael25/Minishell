/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikaismou <ikaismou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 15:56:55 by ikaismou          #+#    #+#             */
/*   Updated: 2023/02/19 17:19:05 by ikaismou         ###   ########.fr       */
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

int	exec_cmd(t_minishell *ms, char **envp)
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
}


/////////////////////////////////////VERSION PIPE NON FINI//////////////////////////////////////////

/*int	exec_cmd(t_minishell *ms, char **envp)
{
	char	**tmp;
	char	**tmp2;
	int		id;

	(void)(envp);
	tmp = ft_split(ms->line, '\n');
	tmp2 = ft_split(tmp[0], '|');
	int i = 0;

	while (tmp2[i])
	{
		ms->input_cmd = ft_split(tmp2[i], ' ');
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
		wait(NULL);
		wait(NULL);
		i++;
	}
	ms->prompt = "👨‍💻 Minishell> ";
	return (1);
}*/