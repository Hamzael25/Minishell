/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikaismou <ikaismou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 15:54:37 by ikaismou          #+#    #+#             */
/*   Updated: 2023/02/19 16:30:28 by ikaismou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*ft_find_path(char **envp)
{
	int	i;

	i = 0;
	while (ft_strncmp("PATH", envp[i], 4))
		i++;
	if (!envp[i])
		return (NULL);
	return (envp[i] + 5);
}

int	init_env(t_minishell *ms, char **envp)
{
	char	*path;

	path = ft_find_path(envp);
	if (!path)
		return (0);
	ms->path_env = ft_split(path, ':');
	if (!ms->path_env)
		return (0);
	return (1);
}
