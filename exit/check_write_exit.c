/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_write_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikaismou <ikaismou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 16:03:58 by ikaismou          #+#    #+#             */
/*   Updated: 2023/02/19 17:53:29 by ikaismou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_write_exit(t_minishell *ms)
{
	if (!ft_strncmp(ms->line, "exit", 4))
	{
		write(1, "👳‍ <== SALAM\n", ft_strlen("👳‍ <== SALAM\n"));
		return (0);
	}
	return (1);
}
