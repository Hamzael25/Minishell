/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikaismou <ikaismou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 23:52:48 by ikaismou          #+#    #+#             */
/*   Updated: 2023/02/25 17:52:35 by ikaismou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_find_path(t_env **env, char *srch)
{
	t_env	*head;

	head = *env;
	while (head)
	{
		if (!ft_strncmp(head->str, srch, ft_strlen(srch)))
			return (head->str + (ft_strlen(srch) + 1));
		head = head->next;
	}
	return (NULL);
}