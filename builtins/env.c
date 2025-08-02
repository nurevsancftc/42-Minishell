/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:08:27 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/03 01:28:52 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h> // printf kullanmak için

/**
 * ft_env - 'env' yerleşik komutunu çalıştırır.
 *
 * Ortam değişkenleri listesini gezer ve her birini ekrana basar.
 *
 * Adımlar:
 * 1. Argüman kontrolü yapar. `env` komutu argüman almaz. Eğer
 *    `args[1]` mevcutsa, bir hata mesajı basar ve 1 döndürür.
 * 2. Bir döngü ile `shell->env_list`'in başından sonuna kadar gider.
 * 3. Her döngü adımında, `t_list` düğümünün `content`'ini bir `t_env`
 *    yapısına dönüştürür (casting).
 * 4. Değişkenin bir değeri varsa (value'su NULL değilse), onu
 *    "ANAHTAR=DEĞER" formatında ekrana basar.
 *
 * Parametreler:
 *   args  - Komut ve argümanlarını içeren dizi.
 *   shell - Kabuğun genel durumunu ve env_list'i içeren ana yapı.
 *
 * Döner Değer:
 *   Başarılı olursa 0, hata (fazla argüman) olursa 1.
 */
int	ft_env(char **args, t_shell *shell)
{
	t_list	*current_node;
	t_env	*current_env;

	// 1. Argüman Kontrolü
	// Eğer `args[1]` varsa, bu "env" komutundan sonra başka bir
	// kelime yazıldığı anlamına gelir. Bu bir hatadır.
	if (args[1] != NULL)
	{
		write(2, "env: too many arguments\n", 24);
		return (1); // Hata kodu döndür.
	}

	// 2. Ortam Değişkenleri Listesini Gez ve Yazdır
	// Listenin başını bir geçici pointer'a ata.
	current_node = shell->env_list;
	while (current_node != NULL)
	{
		// En önemli adım: `void *content`'i `t_env *`'a dönüştür.
		// Artık `key` ve `value`'ya erişebiliriz.
		current_env = (t_env *)current_node->content;
		
		// bash davranışı, değeri olan değişkenleri yazdırmaktır.
		// `export VAR` ile oluşturulmuş ama değeri atanmamış değişkenler
		// genellikle `env` çıktısında görünmez.
		if (current_env && current_env->value != NULL)
		{
			printf("%s=%s\n", current_env->key, current_env->value);
		}
		
		// Listedeki bir sonraki düğüme geç.
		current_node = current_node->next;
	}

	return (0); // İşlem başarılı.
}