/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:38:37 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/04 18:38:54 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parser/print_tokens.c (veya utils/debug_utils.c)

#include "minishell.h"

// Token tipini (enum) okunabilir bir string'e çeviren yardımcı fonksiyon.
static const char	*token_type_to_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	if (type == TOKEN_PIPE)
		return ("PIPE");
	if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == TOKEN_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	return ("UNKNOWN");
}

/*
 * print_tokens - Hata ayıklama (debugging) için bir token listesini yazdırır.
 *
 * Bu fonksiyon, lexer'ın çıktısını kontrol etmek için çok kullanışlıdır.
 * Her token'ı, tipini ve değerini görsel olarak düzenli bir formatta basar.
 */
void	print_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	// Liste boşsa belirt.
	if (!tokens)
	{
		printf("Token list is empty.\n");
		return;
	}

	printf("--- TOKEN LIST ---\n");
	// Listenin sonuna kadar (NULL görene kadar) döngüye gir.
	while (tokens)
	{
		// Her token için bilgilerini yazdır.
		// %-4d: Sola yaslı, 4 karakterlik yer kaplayan onluk sayı (indeks)
		// %-15s: Sola yaslı, 15 karakterlik yer kaplayan string (token tipi)
		// '%s': Değerin kendisi
		printf("Token[%-2d] | Type: %-15s | Value: '%s'\n", i,
			token_type_to_str(tokens->type), tokens->value);
		
		// Bir sonraki token'a geç.
		tokens = tokens->next;
		i++;
	}
	printf("--- END OF LIST ---\n");
}