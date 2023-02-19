/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikaismou <ikaismou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 15:34:03 by ikaismou          #+#    #+#             */
/*   Updated: 2023/02/19 16:54:27 by ikaismou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell	ms;

	ms.prompt = PROMPT;
	(void)(argv);
	if (argc > 1)
		return (perror("Number of Arguments"), 0);
	init_env(&ms, envp);
	ms.line = NULL;
	while (1)
	{
		if (write(1, ms.prompt, ft_strlen(ms.prompt)) == -1)
				return (0);
		if (ms.line)
		{
			free(ms.line);
			ms.line = NULL;
		}
		ms.line = get_next_line_gnl(0);
		if (ms.line && ms.line[0] == '\n')
			continue;
		if (!check_write_exit(&ms))
			exit(0);
		exec_cmd(&ms, envp);
	}
}
