/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:51:08 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/05 19:55:07 by aldurmaz         ###   ########.fr       */
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
	t_list	*new_node;
	t_token	*redir_token;
	t_token	*delimiter_token;
	char	*original_delimiter;

	redir_token = *token_cursor;
	delimiter_token = redir_token->next; // Delimiter'ın olduğu token

	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (-1);

	redir->type = redir_token->type;
	original_delimiter = delimiter_token->value;

	// HEREDOC İÇİN ÖZEL MANTIK
	if (redir->type == TOKEN_HEREDOC)
	{
		// Delimiter'ın ilk karakteri tırnak mı?
		if (original_delimiter[0] == '\'' || original_delimiter[0] == '"')
			redir->expand_in_heredoc = 0; // Tırnak varsa genişletme YAPMA.
		else
			redir->expand_in_heredoc = 1; // Tırnak yoksa genişletme YAP.
		
		// Delimiter'dan tırnakları temizle. "eof" -> eof
		redir->filename = ft_strunquote(original_delimiter);
	}
	else // Diğer yönlendirmeler için (<, >, >>)
	{
		redir->expand_in_heredoc = 0; // Bu alanın bir anlamı yok ama sıfırlayalım.
		redir->filename = ft_strdup(original_delimiter);
	}

	if (!redir->filename) // strdup veya strunquote hatası
		return (free(redir), -1);

	// Oluşturulan yapıyı listeye ekle.
	new_node = ft_lstnew(redir);
	if (!new_node)
	{
		free(redir->filename);
		free(redir);
		return (-1);
	}
	ft_lstadd_back(&cmd->redirections, new_node);

	// İmleci 2 token ileri taşı.
	*token_cursor = delimiter_token->next;
	return (0);
}

static int	validate_and_count_args(t_token *token, int *arg_count)
{
	*arg_count = 0;
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_WORD)
			(*arg_count)++;
		else // Bir yönlendirme token'ı ise
		{
			// Hata: Yönlendirme operatöründen sonra token yoksa veya
			// o token bir kelime değilse, bu bir syntax hatasıdır.
			if (!token->next || token->next->type != TOKEN_WORD)
			{
				printf("minishell: syntax error near unexpected token `");
				if (token->next)
					printf("%s'\n", token->next->value);
				else
					printf("newline'\n");
				return (-1);
			}
			// Yönlendirme ve dosya adını atla (2 token ileri git)
			token = token->next;
		}
		token = token->next;
	}
	return (0); // Başarılı, hata yok.
}

// GÜNCELLENMİŞ ANA FONKSİYON
int	populate_simple_cmd(t_simple_command *cmd, t_token **token_cursor)
{
	int	arg_count;
	int	i;

	// 1. AŞAMA: Doğrula ve Say.
	// Hata varsa, hiçbir bellek ayırmadan çık.
	if (validate_and_count_args(*token_cursor, &arg_count) == -1)
		return (-1);

	// 2. AŞAMA (Başarılıysa): Belleği Ayır.
	// calloc kullanmak, dizinin içini NULL ile dolduracağı için daha güvenlidir.
	cmd->args = ft_calloc(arg_count + 1, sizeof(char *));
	if (!cmd->args)
		return (-1); // Malloc hatası

	// 3. AŞAMA: Yapıları Doldur.
	// Bu aşamada syntax hatası olmayacağını biliyoruz, bu yüzden güvenli.
	i = 0;
	while (*token_cursor && (*token_cursor)->type != TOKEN_PIPE)
	{
		if ((*token_cursor)->type == TOKEN_WORD)
			cmd->args[i++] = ft_strdup((*token_cursor)->value);
		else // Yönlendirme ise, doldurma işlemini yap.
		{
			// handle_redirection muhtemelen içinde malloc yapıyordur.
			// Orada bir hata olursa yine -1 ile çıkılmalı.
			if (handle_redirection(token_cursor, cmd) == -1)
				return (-1); // handle_redirection içindeki malloc hatası
			continue; // handle_redirection imleci zaten ilerletti.
		}
		*token_cursor = (*token_cursor)->next;
	}
	return (0); // Her şey başarılı.
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
		current_cmd_node = calloc(1, sizeof(t_command_chain));
		if (!current_cmd_node)
			return (NULL); // malloc error, Hata yönetimi: Önceki listeyi temizle
		current_cmd_node->simple_command = create_simple_cmd();
		if (!current_cmd_node->simple_command)
		{
    		// create_simple_cmd başarısız oldu
    		free(current_cmd_node); // Sadece o anki düğümü temizle
    		free_cmd_tree(cmd_head); // Öncekileri temizle
    		return (NULL);
		}
		current_cmd_node->next = NULL;

		// Asıl doldurma işlemini yapan ve imleci ilerleten fonksiyon.
		if(populate_simple_cmd(current_cmd_node->simple_command, &current_token) == -1) // &tokens kısmı yanlış olabilir kontrol et!
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
