/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 03:50:10 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/07/27 15:13:47 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * LEXER ANA FONKSİYONU
 * Görevi: Ham komut satırını alır ve bir token listesi döndürür.
 * 1. Boş bir token listesi başlatır.
 * 2. Satırın başından sonuna kadar bir döngü ile ilerler.
 * 3. Her adımda:
 *    a. Boşlukları atlar.
 *    b. Eğer metakarakter (| < >) görürse, handle_metachar() fonksiyonunu çağırır.
 *    c. Eğer alıntı (' veya ") görürse, handle_quotes() fonksiyonunu çağırır.
 *    d. Diğer durumlarda (normal harf/rakam), bunu bir kelimenin başlangıcı
 *       olarak kabul eder ve handle_word() fonksiyonunu çağırır.
 * 4. Helper fonksiyonlar (handle_...) işledikleri kısmın uzunluğunu döndürür.
 *    Döngü bu uzunluk kadar ilerletilir.
 * 5. Satırın sonuna gelindiğinde, oluşturulan token listesinin başını döndürür.
*/

#include "minishell.h"
#include "parser.h"

t_token	*lexer(const char *line)
{
	t_token	*token_list_head; // Token listemizin başı
	int		i;     // Satırda ilerlemek için index

	token_list_head = NULL;
	i = 0;
	
	// 1. Boşlukları atla.
	// 2. Metakarakterleri kontrol et (|, <, >).
	// 3. Kelimeleri (tırnak işaretleri dahil) ayır.
	// 4. Her parça için create_new_token çağır ve listeye ekle.

	// Oluşturulan token listesinin başlangıcını döndür.
	return (token_list_head);
}

