/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:54:53 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/07/27 19:27:43 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parser/quotes.c

#include "minishell.h"

/*
 * HANDLE_QUOTES FONKSİYONU
 * Görevi: Tek veya çift tırnakla başlayan bir bölümü bir bütün olarak işlemek.
 * 1. Başlangıç tırnağını kaydeder.
 * 2. Eşleşen kapanış tırnağını arar.
 * 3. Kapanış tırnağı bulunamazsa, sözdizimi hatası verir (-1 döndürür).
 * 4. Tırnaklar dahil tüm bölümü kopyalar ve bir T_WORD token'ı oluşturur.
 *    (Tırnakları kaldırma işi expander'a aittir).
 * 5. İşlenen toplam karakter sayısını döndürür.
*/
int	handle_quotes(const char *line, t_token **token_list)
{
	char	quote_char;
	int		i;
	char	*content;

	quote_char = line[0];
	i = 1;
	// Eşleşen tırnağı bulana veya satır sonuna gelene kadar ilerle
	while (line[i] && line[i] != quote_char)
		i++;
	
	// Kapanmamış tırnak hatası
	if (line[i] == '\0')
	{
		printf("minishell: syntax error: unclosed quote\n");
		return (-1); // Hata kodu
	}
	
	i++; // Kapanış tırnağını da dahil et
	
	// Tırnaklar dahil tüm metni kopyala
	content = ft_substr(line, 0, i);
	add_token_to_list(token_list, create_token(content, TOKEN_WORD));
	free(content);

	// İşlenen toplam karakter sayısını döndür
	return (i);
}