/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:15:29 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/05 18:18:32 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



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
	t_env	*env_var;

	node = find_env_node(env_list, key);
	if (node == NULL)
	{
		return (NULL); // Değişken listede yok.
	}
	
	env_var = (t_env *)node->content;

	// EKSTRA KONTROL: Değişken var ama değeri NULL mı? (örn: export VAR)
	if (env_var->value == NULL)
	{
		return (NULL); // veya return ("") de bir seçenek olabilir.
	}

	return (env_var->value);
}


/**
 * free_env_content - `ft_lstdelone` için bir `t_env` içeriğini temizler.
 */
void	free_env_content(void *content)
{
	t_env	*env;

	env = (t_env *)content;
	if (!env)
		return;
	free(env->key);
	free(env->value);
	free(env);
}

/**
 * is_valid_identifier - Bir anahtarın geçerli bir kabuk adı olup olmadığını
 *                       kontrol eder. (export ve unset kullanabilir)
 *                       Not: Bu fonksiyonu `export` için yazdığımızdan
 *                       farklı olarak, '=' kontrolü yapmaz.
 *                       Daha genel bir isim verilebilir.
 */
int	is_valid_identifier(const char *key)
{
	int i;

	i = 0;
	if (!key || !(ft_isalpha(key[i]) || key[i] == '_'))
		return (0);
	i++;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/**
 * handle_dollar_sign - Bir '$' karakteriyle karşılaşıldığında çağrılır.
 * Değişkeni okur, değerini bulur ve döndürür.
 */
char	*handle_dollar_sign(const char **word_ptr, t_shell *shell)
{
	const char	*start;
	char		*key;
	char		*value;

	(*word_ptr)++; // '$' karakterini atla

	if (**word_ptr == '?')
	{
		(*word_ptr)++;
		return (ft_itoa(shell->exit_code));
	}
	
	start = *word_ptr;
	// Değişken adının sonunu bul (alphanumeric veya _)
	while (ft_isalnum(**word_ptr) || **word_ptr == '_')
		(*word_ptr)++;

	if (start == *word_ptr) // Sadece '$' varsa
		return (ft_strdup("$"));
		
	key = ft_substr(start, 0, *word_ptr - start);
	value = get_env_value(shell->env_list, key); // Ortam değişkenlerinden değeri al
	free(key);

	if (!value) // Değişken bulunamadıysa boş string döndür
		return (ft_strdup(""));
	
	return (ft_strdup(value));
}