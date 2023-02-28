/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikaismou <ikaismou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 15:54:37 by ikaismou          #+#    #+#             */
/*   Updated: 2023/02/25 18:03:40 by ikaismou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	init_env(t_minishell *ms, char **envp, t_env **env)
{
	t_env	*cell;
	char *path;
	int		i;

	i = 0;
	while (envp[i])
	{
		cell = create_cell(envp[i]);
		if (!cell)
		{
			lstclear(env);
			return (0);
		}
		ft_lstad_back(env, cell);
		i++;
	}
 	path = ft_find_path(env, "PATH");
	if (!path)
		return (0);
	ms->path_env = ft_split(path, ':');
	if (!ms->path_env)
		return (0);
	return (1);
}