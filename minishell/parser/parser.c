/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:51:08 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/03 01:15:10 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Ana Parser Dosyası

// Bu dosya, token listesini alıp komut ağacını inşa eden ana mantığı içerir.

// Parser'ın Mantığı:

// 1.  Ana `parser` fonksiyonu, token listesini baştan sona gezer.
// 2.  Her `PIPE` token'ı gördüğünde veya listenin sonunda, bir "basit komut" grubunun bittiğini anlar.
// 3.  Bir basit komut grubunu işlemek için bir yardımcı fonksiyon çağırır.
// 4.  Bu yardımcı, token'ları `args` (argümanlar) ve `redirections` (yönlendirmeler) olarak ayırır.
// 5.  Sonuç olarak, birbirine `next` ile bağlanmış `t_command_chain` düğümlerinden oluşan bir liste (bizim AST'miz) oluşturur.,


#include "minishell.h"

// Yeni bir t_command_node yapısı oluşturur ve içini başlatır.
static t_simple_command	*create_simple_cmd(void)
{
	t_simple_command *cmd;

	cmd = malloc(sizeof(t_simple_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirections = NULL;
	return (cmd);
}

// Oluşturulan yeni komut düğümünü zincirin sonuna ekler.
static void	add_cmd_to_chain(t_command_chain **head, t_command_chain *new_node)
{
	t_command_chain	*current;

	if (!*head)
	{
		*head = new_node;
		return;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
/*
 * Bir yönlendirme token'ı ve onun dosya adını işler.
 * Başarılı olursa 0, syntax hatası olursa -1 döndürür.
 */
static int	handle_redirection(t_token **token_cursor, t_simple_command *cmd)
{
	t_redir	*redir;
	t_list	*new_list_node;
	t_token	*redir_token;

	redir_token = *token_cursor;
	// SYNTAX CHECK: Yönlendirmeden sonra bir token var mı?
	if (!redir_token->next || redir_token->next->type != TOKEN_WORD)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1); // Hata
	}
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (-1); // Malloc hatası
	redir->type = redir_token->type;
	redir->filename = ft_strdup(redir_token->next->value);
	
	new_list_node = ft_lstnew(redir);
	ft_lstadd_back(&cmd->redirections, new_list_node);
	
	// İşaretçiyi hem yönlendirme hem de dosya adı token'ını geçecek şekilde ilerlet.
	*token_cursor = (*token_cursor)->next->next;
	return (0); // Başarılı
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
int	populate_simple_cmd(t_simple_command *cmd, t_token **token_cursor)
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

	int		arg_count;
	t_token	*temp_cursor;
	int		i;

	arg_count = 0;
	temp_cursor = *token_cursor;
	
	// 1. ADIM: Argüman sayısını öğrenmek için pipe görene kadar token'ları say (yer ayırmak için).
	while (temp_cursor && temp_cursor->type != TOKEN_PIPE)  //Daha sonra count_args ile ayrılacak.
	{
		if (temp_cursor->type == TOKEN_WORD)
			arg_count++;
		else // Yönlendirmeyse, hem kendisini hem dosya adını atla
			temp_cursor = temp_cursor->next;
		temp_cursor = temp_cursor->next;
	}

	// 2. ADIM: Argüman dizisi için yer ayır (+1 NULL için).
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (-1); // Malloc hatası

	
	// 3. ADIM: Argümanları ve yönlendirmeleri doldur.
	i = 0;
	while (*token_cursor && (*token_cursor)->type != TOKEN_PIPE)
	{
		if ((*token_cursor)->type == TOKEN_WORD)
			cmd->args[i++] = ft_strdup((*token_cursor)->value);
		else // Yönlendirme ise, özel yardımcıyı çağır.
		{
			if (handle_redirection(token_cursor, cmd) == -1)
				return (-1); // Hata durumunda çık.
			continue; // `handle_redirection` imleci zaten ilerletti.
		}
		*token_cursor = (*token_cursor)->next;
	}
	cmd->args[i] = NULL; // Diziyi NULL ile sonlandır.
	return (0); // Başarılı
}

/*
 * PARSER ANA FONKSİYONU
 * Görevi: Token listesini alır ve bir komut ağacı (t_command_chain listesi) döndürür.
 * 1. Boş bir komut ağacı başlatır.
 * 2. Token listesinde bir döngü ile ilerler.
 * 3. Her döngü adımında yeni bir komut tablosu düğümü oluşturur.
 * 4. Bu düğümün içini doldurması için populate_command_node'i çağırır.
 * 5. Eğer bir pipe '|' token'ı varsa, onu atlar ve döngüye devam eder.
 * 6. Sözdizimi hatalarını kontrol eder (örn: pipe ile başlayan veya biten komutlar).
*/
t_command_chain	*parser(t_token *tokens)
{
	t_command_chain	*cmd_head;
	t_command_chain	*current_cmd_node;
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
		current_cmd_node = malloc(sizeof(t_command_chain));
		if (!current_cmd_node)
			return (NULL); // malloc error, Hata yönetimi: Önceki listeyi temizle
		current_cmd_node->simple_command = create_simple_cmd();
		current_cmd_node->next = NULL;

		// Asıl doldurma işlemini yapan ve imleci ilerleten fonksiyon.
		if(populate_simple_cmd(current_cmd_node->simple_command, &tokens) == -1); // &tokens kısmı yanlış olabilir kontrol et!
		{
			free_cmd_tree(current_cmd_node);
			free_cmd_tree(cmd_head);
			return (NULL);
		}
		
		// Oluşturulan komut düğümünü listeye ekle
		// ... ft_lstadd_back benzeri bir mantıkla `cmd_head` listesine eklenir ...
		add_cmd_to_chain(&cmd_head, current_cmd_node);
		
		if (current_token && current_token->type == TOKEN_PIPE)
		{
			current_token = current_token->next;
			// Syntax hatası: Komut `|` ile bitemez veya `| |` olamaz.
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



/*
** Fonksiyon İsimleri:
** - 'populate_command_node' gibi fonksiyon isimleri senin orijinal iskeletine sadık kalınarak kullanılmıştır.
** - 't_command_chain', 't_cmd_table' ile aynı yapıdadır; sadece isimlendirme farklıdır.
*/

/*
** Token İşaretçisi:
** - 'populate_command_node' gibi fonksiyonlara '&tokens' yerine '&current_token' gönderilir.
** - Böylece fonksiyon, işlenen token sonrası kaldığı yerden devam eder.
** - Bu pointer ilerlemesi parser’ın düzgün çalışması açısından kritik önemdedir.
*/

/*
** Fonksiyonel Ayrım:
** - 'populate_simple_cmd' ve 'handle_redirection' gibi fonksiyonlara bölündü.
** - Bu yapı, kodun okunabilirliğini ve yönetilebilirliğini artırır.
** - Ayrıca Tek Sorumluluk Prensibi’ne (SRP) uygundur.
*/

/*
** Hata Yönetimi:
** - 'populate_simple_cmd' ve 'handle_redirection' başarısız olursa -1 döner.
** - Ana parser fonksiyonu bu durumu kontrol eder.
** - Syntax hatasında şimdiye kadar ayrılan belleği temizleyip NULL döner.
** - Bu sayede bellek sızıntıları önlenmiş olur.
*/

/*
** Pipe Kontrolleri:
** - Komut başında pipe olup olmadığı kontrol edilir. ('| ls' hatalıdır)
** - 'populate_simple_cmd' çağrıldıktan sonra:
**     - Eğer sıradaki token PIPE ise, ondan sonra gelen tokenın da geçerli olması gerekir.
**     - Yan yana pipe'lar veya pipe sonrası boşluk hataları yakalanır. ('ls | | wc' gibi)
*/

/*
** Redirection Kontrolü:
** - '>' veya '<' gibi bir yönlendirme token'ı varsa,
**     - hemen ardından bir filename gelmelidir.
**     - Bu kontrol, tam olarak ait olduğu yerde yani 'handle_redirection' fonksiyonu içinde yapılır.
*/
