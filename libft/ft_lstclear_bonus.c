/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:36:49 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/04 17:05:15 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (lst == NULL || *lst == NULL)
		return ;
	while (*lst)
	{
		if (del != NULL && (*lst)->content != NULL)
			del((*lst)->content);
		temp = *lst;
		*lst = temp->next;
		free(temp);
	}
	*lst = NULL;
}
