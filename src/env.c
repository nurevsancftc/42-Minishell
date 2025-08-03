/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:49:01 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/03 09:35:33 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// src/environment.c

#include "minishell.h"

/*
 * Tek bir "KEY=VALUE" string'ini alır ve bir t_env veri paketi oluşturur.
 * Bu bir liste düğümü DEĞİLDİR.
 */
static t_env	*create_env_node(char *env_line)
{
	t_env	*env_node;
	char	*equals_pos;

	env_node = malloc(sizeof(t_env));
	if (!env_node)
		return (NULL); // Malloc hatası

	equals_pos = ft_strchr(env_line, '=');
	if (!equals_pos) // Eğer '=' yoksa (geçersiz format), key'i tüm satır yap
	{
		env_node->key = ft_strdup(env_line);
		env_node->value = NULL; // Değeri yok
	}
	else
	{
		// '=' işaretinden önceki kısmı 'key' olarak al
		env_node->key = ft_substr(env_line, 0, equals_pos - env_line);
		// '=' işaretinden sonraki kısmı 'value' olarak al
		env_node->value = ft_strdup(equals_pos + 1);
	}

	// Herhangi bir malloc başarısız olduysa temizlik yap
	if (!env_node->key || (!env_node->value && equals_pos))
	{
		free(env_node->key);
		free(env_node->value);
		free(env_node);
		return (NULL);
	}
	return (env_node);
}

/*
 * main'den gelen char **envp dizisini alır ve t_env yapısında
 * bir bağlı liste oluşturup döndürür.
 * Her liste elemanının (node) içeriği (content) bir t_env yapısıdır.
 */
// src/env.c (sadece create_env_list fonksiyonu)
t_list	*create_env_list(char **envp) // Dönüş tipini t_list* yap
{
	t_list	*env_list_head; // Tipi t_list*
	t_list	*new_list_node; // Tipi t_list*
	t_env	*env_node_content;
	int		i;

	env_list_head = NULL;
	i = 0;
	while (envp[i])
	{
		env_node_content = create_env_node(envp[i]);
		if (!env_node_content)
		{
			ft_lstclear(&env_list_head, free_env_content);
			return (NULL);
		}
		new_list_node = ft_lstnew(env_node_content);
		if (!new_list_node)
		{
			free_env_content(env_node_content);
			ft_lstclear(&env_list_head, free_env_content);
			return (NULL);
		}
		ft_lstadd_back(&env_list_head, new_list_node);
		i++;
	}
	return (env_list_head);
}