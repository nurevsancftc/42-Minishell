/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:36:49 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/08 07:22:41 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*next_node;

	if (!lst || !del) // Koruma: liste veya silme fonksiyonu NULL ise hiçbir şey yapma
		return;
	current = *lst;
	while (current != NULL)
	{
		next_node = current->next; // Bir sonraki düğümü kaydet
		ft_lstdelone(current, del); // Mevcut düğümü ve içeriğini sil
		current = next_node;        // Kaydedilen bir sonraki düğüme geç
	}
	*lst = NULL; // Listenin başını NULL yap, artık boş.
}
