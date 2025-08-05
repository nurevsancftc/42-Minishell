/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:58:42 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/05 20:19:55 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * expand_word - Tek bir kelimeyi (argüman veya dosya adı) genişletir.
 * Değişkenleri değiştirir ve tırnakları kaldırır.
 */
char	*expand_word(const char *word, t_shell *shell)
{
	char	*new_word; // Genişletilmiş yeni kelimeyi burada biriktireceğiz
	char	quote_type; // ' veya " veya 0 (tırnak içinde değil)

	new_word = ft_strdup(""); // Boş bir string ile başla
	quote_type = 0;

	while (*word)
	{
		if (quote_type == 0 && (*word == '\'' || *word == '"'))
		{
			// Tırnak moduna gir
			quote_type = *word;
			word++;
		}
		else if (quote_type != 0 && *word == quote_type)
		{
			// Tırnak modundan çık
			quote_type = 0;
			word++;
		}
		else if (quote_type != '\'' && *word == '$')
		{
			// Değişken genişletme (tek tırnak içinde değilse)
			char *expanded_var = handle_dollar_sign(&word, shell);
			char *temp = ft_strjoin(new_word, expanded_var);
			free(new_word);
			free(expanded_var);
			new_word = temp;
		}
		else
		{
			// Normal bir karakter, yeni kelimeye ekle
			char temp_str[2] = {*word, '\0'};
			char *temp = ft_strjoin(new_word, temp_str);
			free(new_word);
			new_word = temp;
			word++;
		}
	}
	return (new_word);
}

/**
 * is_originally_quoted - Bir string'in orijinal halinin tırnakla başlayıp
 *                        bittiğini kontrol eder. Bu, expander'dan önce
 *                        olan durumu anlamak içindir.
 *                        NOT: Bu, basit bir kontroldür. Karmaşık durumlar
 *                        (örn: "a"'b') için daha gelişmiş bir analiz gerekir.
 */
static int	is_originally_quoted(const char *original_word)
{
	int len;

	if (!original_word)
		return (0);
	len = ft_strlen(original_word);
	if (len >= 2)
	{
		if ((original_word[0] == '\'' && original_word[len - 1] == '\'') || \
			(original_word[0] == '"' && original_word[len - 1] == '"'))
			return (1);
	}
	return (0);
}
void	expand_redirections(t_simple_command *cmd, t_shell *shell)
{
	t_list	*redir_node;
	t_redir	*redir;
	char	*original_filename;

	redir_node = cmd->redirections;
	while (redir_node)
	{
		redir = (t_redir *)redir_node->content;
		// Heredoc delimiter'ı ASLA genişletilmez.
		// Bizim mantığımızda heredoc'lar zaten işlendiği ve tipi
		// TOKEN_REDIR_IN'e çevrildiği için bu kontrol yine de önemlidir.
		// Eğer parser'da tipi değiştirmediyseniz, burada `if (redir->type != TOKEN_HEREDOC)` kontrolü yapın.
		if (redir->filename)
		{
			original_filename = redir->filename;
			redir->filename = expand_word(original_filename, shell);
			free(original_filename);

			// ÖNEMLİ HATA KONTROLÜ:
			// Eğer genişletme sonucu 'ambiguous redirect' hatası oluşursa
			// (örn: `>$VAR` ve VAR="a b" ise), hata verilmeli.
			if (ft_strchr(redir->filename, ' ') || redir->filename[0] == '\0')
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(original_filename, 2); // Kullanıcının ne yazdığını göster
				ft_putstr_fd(": ambiguous redirect\n", 2);
				// Hata durumu yönetimi (örn: shell->exit_code = 1)
				// ve muhtemelen komutun çalışmasını durdurma.
			}
		}
		redir_node = redir_node->next;
	}
}

/**
 * expand_simple_command - Tek bir komutun argümanlarını ve yönlendirmelerini genişletir.
 */
void	expand_simple_command(t_simple_command *cmd, t_shell *shell)
{
	t_list	*arg_list;
	t_list	*current_node;
	t_list	*prev_node;
	char	*original_word;
	char	*expanded_word;

	if (!cmd || !cmd->args)
		return;

	// 1. ADIM: Mevcut char**'ı geçici bir listeye dönüştür.
	arg_list = array_to_list(cmd->args);
	if (!arg_list) return; // Malloc hatası

	// 2. ADIM: Liste üzerinde genişletme yap ve boş argümanları temizle.
	current_node = arg_list;
	prev_node = NULL;
	while (current_node)
	{
		original_word = (char *)current_node->content;
		expanded_word = expand_word(original_word, shell);
		current_node->content = expanded_word;

		// KURAL: Eğer genişletme sonucu boş bir string ise VE orijinal kelime
		// tırnak içinde değilse, bu argümanı listeden tamamen sil.
		if (expanded_word[0] == '\0' && !is_originally_quoted(original_word))
		{
			t_list *node_to_delete = current_node;
			current_node = current_node->next; // İmleci ilerlet
			
			if (prev_node)
				prev_node->next = node_to_delete->next;
			else
				arg_list = node_to_delete->next; // Listenin başı değişti
			
			// Düğümü ve içeriğini (boş string) temizle
			free(original_word); // Orijinal kelimeyi her zaman free et
			ft_lstdelone(node_to_delete, free);
		}
		else
		{
			free(original_word); // Orijinal kelimeyi her zaman free et
			prev_node = current_node;
			current_node = current_node->next;
		}
	}

	// 3. ADIM: Yönlendirmeleri genişlet (heredoc hariç).
	// Bu kısım ayrı bir döngüde ele alınmalı.
	expand_redirections(cmd, shell); // Bu yeni yardımcı fonksiyonu varsayalım

	// 4. ADIM: Orijinal char**'ı temizle ve yerine yeni, filtrelenmiş olanı koy.
	free_args(cmd->args);
	cmd->args = list_to_array(arg_list);

	// 5. ADIM: Geçici listeyi ve içindeki string'leri temizle.
	ft_lstclear(&arg_list, free);
}

/**
 * expander - Komut ağacını gezer ve her bir komut düğümünü genişletir.
 */
void	expander(t_command_chain *cmd_chain, t_shell *shell)
{
	while (cmd_chain)
	{
		expand_simple_command(cmd_chain->simple_command, shell);
		cmd_chain = cmd_chain->next;
	}
}





