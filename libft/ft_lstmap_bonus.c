/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:37:37 by aldurmaz          #+#    #+#             */
/*   Updated: 2024/10/27 18:46:40 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*root;
	t_list	*new;
	void	*tc;

	if (!lst || !f || !del)
		return (NULL);
	root = NULL;
	while (lst)
	{
		tc = f(lst->content);
		if (!tc)
			return (ft_lstclear(&root, del), NULL);
		new = ft_lstnew(tc);
		if (!new)
			return (ft_lstclear(&root, del), del(tc), NULL);
		ft_lstadd_back(&root, new);
		lst = lst->next;
	}
	return (root);
}
