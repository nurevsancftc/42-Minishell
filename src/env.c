/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:49:01 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/07/27 19:27:21 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// src/environment.c

#include "minishell.h"

/*
 * Tek bir "KEY=VALUE" string'ini alır ve bir t_env yapısı oluşturur.
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
t_env	*create_env_list(char **envp)
{
	t_env	*env_list_head;
	t_env	*new_list_node;
	t_env	*env_node_content;
	int		i;

	env_list_head = NULL;
	i = 0;
	// envp dizisi NULL ile sonlanır.
	while (envp[i])
	{
		// 1. "KEY=VALUE" satırından bir t_env yapısı oluştur.
		env_node_content = create_env_node(envp[i]);
		if (!env_node_content)
		{
			ft_lstclear(&env_list_head, free); // Hata durumunda o ana kadar oluşan listeyi temizle
			return (NULL); // ve NULL dön.
		}

		// 2. Bu t_env yapısını content olarak içeren yeni bir t_env düğümü oluştur.
		new_list_node = ft_lstnew(env_node_content);
		if (!new_list_node)
		{
			free(env_node_content->key);
			free(env_node_content->value);
			free(env_node_content);
			ft_lstclear(&env_list_head, free);
			return (NULL);
		}
		
		// 3. Yeni düğümü listemizin sonuna ekle.
		ft_lstadd_back(&env_list_head, new_list_node);
		
		i++;
	}
	return (env_list_head);
}