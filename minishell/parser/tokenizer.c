/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 03:35:40 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/07/27 14:55:29 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// --- TOKEN İŞLEYİCİLER ---

/*
 * HANDLE_WORD FONKSİYONU
 * Görevi: Normal bir kelimeyi (komut/argüman) işlemek.
 * 1. Boşluk, metakarakter veya tırnak görene kadar ilerler.
 * 2. Kelimenin uzunluğunu hesaplar.
 * 3. Kelimeyi kopyalar, T_WORD token'ı oluşturur ve listeye ekler.
 * 4. Kelimenin uzunluğunu döndürür.
*/


/*
 * HANDLE_METACHAR FONKSİYONU
 * Görevi: |, <, >, <<, >> gibi özel karakterleri işlemek.
 * 1. Gelen karakteri kontrol eder.
 * 2. Eğer > veya < ise, bir sonraki karakterin de aynı olup olmadığını
 *    kontrol eder (>> ve << durumları için).
 * 3. Uygun token'ı (örn: T_PIPE, T_REDIR_APPEND) oluşturur ve listeye ekler.
 * 4. İşlenen karakter sayısını (1 veya 2) döndürür.
*/


// Bu fonksiyon, yeni bir token oluşturup listeye eklemek için kullanılacak.
// Şimdilik boş bırakıyoruz, sonra dolduracağız.
static t_token	*create_new_token(char *value, t_token_type type)
{
	t_token *new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL); // Bellek hatası
	new_token->value = value;
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}