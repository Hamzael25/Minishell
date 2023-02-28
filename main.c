/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzaelouardi <hamzaelouardi@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 15:34:03 by ikaismou          #+#    #+#             */
/*   Updated: 2023/02/28 04:26:55 by hamzaelouar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

static void	start_minishell(char **envp, t_minishell *ms, t_env **env)
{
	ms->line = NULL;
	while (1)
	{
		if (ms->parsed)
		{
			free(ms->line);
			ms->line = NULL;
		}
		ms->line = readline(PROMPT);
		if (!ms->line[0])
			continue ;
		check_new_line(ms);
		if (builtins(ms, ms->parsed, envp, env))
			continue ;
		add_history(ms->line);
		if (!check_write_exit(ms))
		{
			ft_free_tab(ms->parsed);
			free(ms->line);
			exit(0);
		}
		if (ms->parsed)
			exec_cmd(ms, envp);
		ft_free_tab(ms->parsed);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	ms;
	t_env		*env;

	(void)argv;
	if (argc > 1)
		return (perror("Number of Arguments"), 0);
	env = NULL;
	init_env(&ms, envp, &env);
	start_minishell(envp, &ms, &env);
	return(0);
}
