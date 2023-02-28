/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzaelouardi <hamzaelouardi@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 14:48:19 by ikaismou          #+#    #+#             */
/*   Updated: 2023/02/28 04:27:35 by hamzaelouar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


///////////////history/////////////////////
static int	input_history(char **split, t_minishell *ms)
{
	int i;
	
	if (split[0] && !split[1] && !ft_strncmp(split[0], "history\0", 8)) 
	{
		i = 0;
		_history(ms->line);
		while (i <= history_length) 
		{
			HIST_ENTRY* entry = history_get(i);
			if (entry != NULL) {
				ft_printf("   %d  %s\n", i, entry->line);
			}
			i++;
		}
		return (1);
	}
	return (0);
}


//////////////////////////////!!!!!!!!!!/////////////////////////
static int	input_last_cmd(char **split, t_minishell *ms, char **envp)
{
	if (split[0] && !split[1] && !ft_strncmp(split[0], "!!\0", 3))
	{
		HIST_ENTRY *last_entry = history_get(history_length);
		if (last_entry) 
		{
			free(ms->line);
			ms->line = last_entry->line;
			add_history(ms->line);
			ft_printf("%s\n", ms->line);
			if (ms->line)
				exec_cmd(ms, envp);
			return (1);
		}
		else
		{
			ft_printf("erreur a mettre \"!!\"\n");
			return (1);
		}
	} 
	return (0);
}

////////////////////////////!!!!!!!!!!xxxxxxxxx////////////////
static int	inputx_index(char **split, t_minishell *ms, char **envp)
{
	if (split[0] && !split[1] && split[0][0] == '!')
	{
		int index;

		*split += 1;
		index = ft_atoi(split[0]);
		if (index > history_length)
		{
			printf("erreur a mette\n");
			return (1);
		}
		HIST_ENTRY *index_hist = history_get(index);
		if (index_hist != NULL) {
			ms->line = index_hist->line;
			_history(ms->line);
		}
		if (ms->line)
			exec_cmd(ms, envp);
		return (1);
	}
	return (0);
}

////////////////////////////ENV////////////////////////
static int	input_env(t_env **env, char **split, t_minishell *ms)
{
	t_env 	*head;

	head = *env;
	if (split[0] && !split[1] && !ft_strncmp(split[0], "env\0", 4))
	{
		while (head)
		{
			ft_printf("%s\n", head->str);
			head = head->next;
		}
		add_history(ms->line);
		return (1);
	}
	return (0);
}

//////////////CCCCCCCCCDDDDDDDDDD////////////
static int	input_cd(char **split, t_minishell *ms, t_env **env)
{
	if (split[0] && !ft_strncmp(split[0], "cd\0", 3))
	{
		if (!split[1])
		{
			if (chdir(ft_find_path(env, "HOME")) == -1)
				ft_printf("erreur a mettre chdir\n");
		}
		else if (chdir(split[1]) == -1 && split[1]) 
			ft_printf("erreur a mettre chdir\n");
		add_history(ms->line);
		return (1);
	}
	return (0);
}


//////////////////PWD//////////////////////

int	built_in_pwd(char **split, t_minishell *ms)
{
	char cwd[100000];
	
	if (split[0] && !split[1] && !ft_strncmp(split[0], "pwd\0", 4))
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			printf("%s\n", cwd);
		} else {
			perror("getcwd()");
		}
		add_history(ms->line);
		return (1);
	}
	return (0);
}


///////////////////export//////////

int built_in_export(t_env **env, char **split, t_minishell *ms)
{
	if (!ft_strncmp(split[0], "export\0,", 1) && ft_strchr(split[1], '='))
	{
		t_env	*cell;
		
		char *m = malloc(sizeof(char) * (ft_strlen(split[1]) + 1));
		int i = 0;
		
		while (split[1][i])
		{
			m[i] = split[1][i];
			i++;
		}
		m[i] = 0;
		cell = create_cell(m);
		if (!cell)
		{
			lstclear(env);
			return (0);
		}
		ft_lstad_back(env, cell);
		add_history(ms->line);
		////free(m);
		return (1);
	}
	return (0);
}

////////////////////////unset/////////////////////////////

int	built_in_unset(t_env **env, char **split)
{
	if (!ft_strncmp(split[0], "unset\0", 6) && split[1])
	{
		t_env *occur;
		
		occur = (*env);
		while (ft_strncmp(occur->str, split[1], ft_strlen(split[1])))
			occur = occur->next;
		free(occur);
		return (1);
	}
	return (0);
}


int builtins(t_minishell *ms, char **split, char **envp, t_env **env)
{
	if (input_history(split, ms) || input_last_cmd(split, ms, envp) 
			|| inputx_index(split, ms, envp) || input_env(env, split, ms) 
			|| input_cd(split, ms, env) || built_in_pwd(split, ms) 
			|| built_in_export(env, split, ms) || built_in_unset(env, split))
	{
		free(ms->line);
		ft_free_tab(split);
		ms->line = NULL;
		return (1);
	}
	return (0);
}