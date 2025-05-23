/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamza <hamza@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 11:19:41 by hel-ouar          #+#    #+#             */
/*   Updated: 2023/05/17 08:16:50 by hamza            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	custom_msg_check_line(char *line, int pos, char c)
{
	char	*npipe;
	int		size;
	int		save;

	save = pos;
	size = 0;
	while (line[save] && line[save] == c)
	{
		size++;
		save++;
	}
	npipe = ft_calloc_parent(sizeof(char), (size + 1), "parsing");
	size = 0;
	while (line[pos] && line[pos] == c)
	{
		npipe[size] = line[pos];
		pos++;
		size++;
	}
	npipe[size] = 0;
	ft_dprintf(""RED""SYNT_ERR" `%s'\n"WHITE"", npipe);
	return (0);
}

void	print_error_export(char *split)
{
	ft_dprintf(""RED"bash: export: `%s': not a valid identifier\n"\
				WHITE"", split);
	g_global.g_status = 1;
}

int	name_exist(t_minishell *ms, char *tab, int count)
{
	int	i;

	i = 0;
	while (ms->f_name[i] && i < count)
	{
		if (!ft_strncmp(tab, ms->f_name[i], ft_strlen(tab)))
			return (0);
		i++;
	}
	return (1);
}
