/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:58:57 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/05 17:36:29 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NOT: Bu fonksiyonun çalışması için, geçici dosyaları daha sonra
// silebilecek bir mekanizmanız olmalı (örn: ana temizlik fonksiyonunda).

/**
 * read_heredoc_to_temp_file - Tek bir heredoc için kullanıcıdan girdi okur.
 *
 * Kullanıcıdan 'delimiter' geline kadar satır satır okur ve bu içeriği
 * geçici bir dosyaya yazar.
 *
 * @delimiter: Heredoc'u sonlandıracak olan kelime.
 * @return: İçeriğin yazıldığı geçici dosyanın adını (malloc ile) döner.
 *          Hata olursa veya kullanıcı Ctrl+D ile çıkarsa NULL döner.
 */
static char	*read_heredoc_to_temp_file(const char *delimiter)
{
	char	*line;
	int		fd;
	char	*tmp_filename; // Benzersiz bir isim olmalı

	// Benzersiz bir geçici dosya adı oluştur (Bu sadece bir örnek)
	tmp_filename = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
	fd = mkstemp(tmp_filename); // Güvenli geçici dosya oluşturur
	if (fd < 0)
		return (free(tmp_filename), NULL);

	while (1)
	{
		line = readline("> ");
		if (!line) // Kullanıcı Ctrl+D'ye bastı
		{
			// Opsiyonel: "warning: here-document delimited by end-of-file"
			break;
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (tmp_filename);
}

/**
 * handle_heredocs - Tüm komut zincirindeki `heredoc`'ları işler.
 *
 * Komutlar çalıştırılmadan önce çağrılır. `<<` operatörlerini bulur,
 * kullanıcıdan girdiyi okuyup geçici dosyalara yazar ve `t_redir`
 * yapısını bu geçici dosyayı kullanacak şekilde günceller.
 *
 * @cmd_chain: İşlenecek komut zincirinin başı.
 * @return: Başarılı olursa 0, hata olursa -1 döner.
 */
int	handle_heredocs(t_command_chain *cmd_chain)
{
	t_list	*redir_list;
	t_redir	*redir;
	t_list	*last_heredoc_node;

	while (cmd_chain)
	{
		// 1. ADIM: Son heredoc'u bul.
		redir_list = cmd_chain->simple_command->redirections;
		last_heredoc_node = NULL;
		while (redir_list)
		{
			if (((t_redir *)redir_list->content)->type == TOKEN_HEREDOC)
				last_heredoc_node = redir_list;
			redir_list = redir_list->next;
		}

		// 2. ADIM: Tüm heredoc'ları işle.
		redir_list = cmd_chain->simple_command->redirections;
		while (redir_list)
		{
			redir = (t_redir *)redir_list->content;
			if (redir->type == TOKEN_HEREDOC)
			{
				if (redir_list != last_heredoc_node) // Bu sonuncu değil mi?
				{
					// Evet, değil. Girdiyi oku ve at.
					char *dummy_file = read_heredoc_to_temp_file(redir->filename);
					if (!dummy_file) return (-1);
					unlink(dummy_file);
					free(dummy_file);
					// EN ÖNEMLİ ADIM: Bu düğümü geçersiz kıl.
					free(redir->filename);
					redir->filename = NULL; 
				}
				else // Bu, son ve geçerli heredoc.
				{
					char *real_file = read_heredoc_to_temp_file(redir->filename);
					if (!real_file) return (-1);
					free(redir->filename);
					redir->filename = real_file;
					// Tipini değiştir ki '<' gibi davransın.
					redir->type = TOKEN_REDIR_IN;
				}
			}
			redir_list = redir_list->next;
		}
		cmd_chain = cmd_chain->next;
	}
	return (0);
}
