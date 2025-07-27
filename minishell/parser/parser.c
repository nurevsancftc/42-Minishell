/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:51:08 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/07/27 19:28:00 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// 3. Adım: `parser/parser.c` (Ana Parser Dosyası)**

// Bu dosya, token listesini alıp komut ağacını inşa eden ana mantığı içerir.

// **Parser'ın Mantığı:**

// 1.  Ana `parser` fonksiyonu, token listesini baştan sona gezer.
// 2.  Her `PIPE` token'ı gördüğünde veya listenin sonunda, bir "basit komut" grubunun bittiğini anlar.
// 3.  Bir basit komut grubunu işlemek için bir yardımcı fonksiyon çağırır.
// 4.  Bu yardımcı, token'ları `args` (argümanlar) ve `redirections` (yönlendirmeler) olarak ayırır.
// 5.  Sonuç olarak, birbirine `next` ile bağlanmış `t_cmd_table` düğümlerinden oluşan bir liste (bizim AST'miz) oluşturur.,

#include "minishell.h"

// Bu fonksiyonlar static olacak, sadece bu dosyada kullanılacaklar.
static t_simple_cmd	*create_simple_cmd(void);
static int			count_args(t_token *start, t_token *end);
static void			populate_simple_cmd(t_simple_cmd *cmd, t_token **current);

/*
 * PARSER ANA FONKSİYONU
 * Görevi: Token listesini alır ve bir komut ağacı (t_cmd_table listesi) döndürür.
 * 1. Boş bir komut ağacı başlatır.
 * 2. Token listesinde bir döngü ile ilerler.
 * 3. Her döngü adımında yeni bir komut tablosu düğümü oluşturur.
 * 4. Bu düğümün içini doldurması için populate_simple_cmd'i çağırır.
 * 5. Eğer bir pipe '|' token'ı varsa, onu atlar ve döngüye devam eder.
 * 6. Sözdizimi hatalarını kontrol eder (örn: pipe ile başlayan veya biten komutlar).
*/
t_cmd_table	*parser(t_token *tokens)
{
	t_cmd_table	*cmd_head;
	t_cmd_table	*current_cmd_node;
	t_token		*current_token;

	if (!tokens)
		return (NULL);
	cmd_head = NULL;
	current_token = tokens;

	// Syntax hatası: Komut `|` ile başlayamaz.
	if (current_token->type == TOKEN_PIPE)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (NULL);
	}

	while (current_token)
	{
		current_cmd_node = malloc(sizeof(t_cmd_table));
		// ... Malloc kontrolü ...
		current_cmd_node->simple_cmd = create_simple_cmd();
		current_cmd_node->next = NULL;

		// Asıl işi yapan fonksiyon
		populate_simple_cmd(current_cmd_node->simple_cmd, &tokens); // &tokens kısmı yanlış olabilir kontrol et!
		
		// Oluşturulan komut düğümünü listeye ekle
		// ... ft_lstadd_back benzeri bir mantıkla `cmd_head` listesine eklenir ...
		
		if (current_token && current_token->type == TOKEN_PIPE)
		{
			current_token = current_token->next;
			// Syntax hatası: Pipe'dan sonra komut gelmeli.
			if (!current_token || current_token->type == TOKEN_PIPE)
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				free_cmd_tree(cmd_head);
				return (NULL);
			}
		}
	}
	return (cmd_head);
}


// Yeni bir t_simple_cmd yapısı oluşturur ve içini başlatır.
static t_simple_cmd	*create_simple_cmd(void)
{
	t_simple_cmd *cmd;

	cmd = malloc(sizeof(t_simple_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirections = NULL;
	return (cmd);
}

// Bir sonraki PIPE'a kadar olan token'ları sayarak `args` dizisi için yer ayırır.
static int	count_args(t_token *start, t_token *end)
{
	int count = 0;
	while (start != end)
	{
		if (start->type == TOKEN_WORD)
			count++;
		// Yönlendirme token'ları argüman sayılmaz, onlar atlanır.
		else if (start->type >= TOKEN_REDIR_IN && start->type <= TOKEN_HEREDOC)
			start = start->next; // Dosya adını da atla
		start = start->next;
	}
	return (count);
}


// Tek bir basit komutun argümanlarını ve yönlendirmelerini doldurur.
static void	populate_simple_cmd(t_simple_cmd *cmd, t_token **current)
{
	// 1. Önce argümanlar için `malloc` ile yer ayırmak üzere argümanları say.
	// 2. Bir döngü ile PIPE'a veya listenin sonuna kadar git.
	// 3. Her döngü adımında:
	//    a. Eğer token T_WORD ise, bunu cmd->args dizisine ekle.
	//    b. Eğer token bir yönlendirme ise (T_REDIR_*):
	//       i.   Yeni bir t_redir yapısı oluştur.
	//       ii.  Yönlendirme tipini ata (örn: T_REDIR_OUT).
	//       iii. Bir sonraki token'ın değerini (dosya adını) kopyala.
	//       iv.  Bu t_redir yapısını cmd->redirections listesine ekle.
	//       v.   Token işaretçisini iki ileri sar.
	// 4. `args` dizisini NULL ile sonlandır.
}