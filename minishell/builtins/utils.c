/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:15:29 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/01 04:15:53 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"



/* ========================================================================== */
/*                   CD İÇİN GEREKLİ YARDIMCI FONKSİYONLAR                    */
/* ========================================================================== */
/*
 * NOT: Bu yardımcı fonksiyonlar normalde projenin genelinde kullanılmak
 * üzere `env_utils.c` gibi ayrı bir dosyada bulunur. Ancak `cd`'nin
 * mantığını tek bir yerde görebilmek için buraya dahil edilmiştir.
 */

/**
 * find_env_node - Ortam değişkenleri listesinde belirli bir anahtarı arar.
 *
 * Bu fonksiyon, `t_list` üzerinde döner ve her düğümün `content`'ini
 * bir `t_env` yapısına cast ederek anahtar karşılaştırması yapar.
 *
 * Parametreler:
 *   env_list: Ortam değişkenlerini tutan listenin başlangıcı.
 *   key: Aranacak anahtar (örn: "HOME", "PWD").
 *
 * Döner Değer:
 *   Anahtarı içeren `t_list` düğümünü bulursa onun adresini, bulamazsa NULL döner.
 */
t_list	*find_env_node(t_list *env_list, const char *key)
{
	t_env	*current_env;

	while (env_list)
	{
		current_env = (t_env *)env_list->content;
		if (current_env && current_env->key && strcmp(current_env->key, key) == 0)
		{
			return (env_list); // Aranan düğüm bulundu.
		}
		env_list = env_list->next;
	}
	return (NULL); // Bulunamadı.
}

/**
 * get_env_value - Bir ortam değişkeninin değerini string olarak döndürür.
 *
 * Önce `find_env_node` ile ilgili düğümü bulur, sonra o düğümün
 * içindeki `value` alanını döndürür.
 */
char	*get_env_value(t_list *env_list, const char *key)
{
	t_list	*node;

	node = find_env_node(env_list, key);
	if (node == NULL)
	{
		return (NULL); // Değişken listede yok.
	}
	// Düğümün content'ini t_env'e cast et ve value'sunu döndür.
	return (((t_env *)node->content)->value);
}