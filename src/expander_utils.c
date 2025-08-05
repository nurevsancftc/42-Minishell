/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:22:25 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/05 18:22:41 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// array_to_list_utils.c

// Bir char** dizisini, her elemanı strdup ile kopyalanmış bir t_list'e çevirir.
t_list	*array_to_list(char **array)
{
	t_list	*list_head;
	t_list	*new_node;
	int		i;

	list_head = NULL;
	i = 0;
	if (!array)
		return (NULL);
	while (array[i])
	{
		new_node = ft_lstnew(ft_strdup(array[i]));
		if (!new_node || !new_node->content)
		{
			ft_lstclear(&list_head, free); // Hata durumunda temizlik
			return (NULL);
		}
		ft_lstadd_back(&list_head, new_node);
		i++;
	}
	return (list_head);
}

// Bir t_list'i, NULL ile sonlanan bir char** dizisine çevirir.
char	**list_to_array(t_list *list_head)
{
	char	**array;
	int		size;
	int		i;

	size = ft_lstsize(list_head);
	array = ft_calloc(size + 1, sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (list_head)
	{
		array[i] = ft_strdup((char *)list_head->content);
		if (!array[i])
		{
			// Hata durumunda temizlik
			while (i > 0)
				free(array[--i]);
			free(array);
			return (NULL);
		}
		i++;
		list_head = list_head->next;
	}
	return (array);
}