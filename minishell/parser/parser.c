/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 03:50:10 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/07/25 04:02:41 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell/includes/parser.h"

#include "minishell.h" // Ana başlık dosyamız, içinde parser.h'ı da barındıracak.
#include "parser.h"    // Özellikle parser yapıları için (isteğe bağlı, minishell.h'da varsa)

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


// Ana lexer fonksiyonu
t_token	*lexer(const char *line)
{
	t_token	*head; // Token listemizin başı
	int		i;     // Satırda ilerlemek için index

	head = NULL;
	i = 0;
	// Şimdilik sadece "hello" kelimesini token'a çeviren basit bir örnek yapalım.
	if (line && strncmp(line, "hello", 5) == 0)
	{
		// strdup, string'i kopyalamak için kullanılır. Orijinal 'line' değişmez.
		// Gerçek implementasyonda kelimeyi dinamik olarak ayırmalısınız.
		head = create_new_token(strdup("hello"), TOKEN_WORD);
	}
	// TODO: Gerçek lexer mantığı buraya gelecek.
	// 1. Boşlukları atla.
	// 2. Metakarakterleri kontrol et (|, <, >).
	// 3. Kelimeleri (tırnak işaretleri dahil) ayır.
	// 4. Her parça için create_new_token çağır ve listeye ekle.

	// Oluşturulan token listesinin başlangıcını döndür.
	return (head);
}