/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:58:57 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/05 14:05:39 by aldurmaz         ###   ########.fr       */
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
	char	*last_heredoc_filename; // Sadece son dosyanın adını tutacak

	while (cmd_chain)
	{
		redir_list = cmd_chain->simple_command->redirections;
		last_heredoc_filename = NULL;

		// 1. ADIM: Bu komuttaki TÜM heredoc'ları işle, ama sadece sonuncuyu sakla.
		while (redir_list)
		{
			redir = (t_redir *)redir_list->content;
			if (redir->type == TOKEN_HEREDOC)
			{
				// Eğer daha önce başka bir heredoc için geçici dosya oluşturduysak,
				// o artık geçersiz, hem adını hem de dosyanın kendisini sil.
				if (last_heredoc_filename)
				{
					unlink(last_heredoc_filename); // Dosyayı diskten sil
					free(last_heredoc_filename);   // Dosya adının string'ini sil
				}
				
				// Yeni heredoc'u oku ve geçici dosyasının adını al.
				last_heredoc_filename = read_heredoc_to_temp_file(redir->filename);
				if (!last_heredoc_filename)
					return (-1); // Hata veya Ctrl+D
			}
			redir_list = redir_list->next;
		}

		// 2. ADIM: Şimdi listeyi tekrar gez ve heredoc'ları güncelle.
		// Bu komut için sadece bir tane geçerli heredoc dosyası var: 'last_heredoc_filename'
		redir_list = cmd_chain->simple_command->redirections;
		while (redir_list)
		{
			redir = (t_redir *)redir_list->content;
			if (redir->type == TOKEN_HEREDOC)
			{
				// Orijinal delimiter'ı free et
				free(redir->filename);
				// Bu heredoc, sonuncu olan mı?
				if (last_heredoc_filename)
				{
					// Evet, sonuncu bu. filename'e geçici dosyanın adını ver
					// ve sahipliğini devret.
					redir->filename = last_heredoc_filename;
					last_heredoc_filename = NULL; // Sahiplik devredildi, artık NULL.
				}
				else
				{
					// Bu olası değil ama bir güvenlik önlemi.
					// Bu, son olmayan bir heredoc. Geçersiz kıl.
					redir->filename = NULL; 
				}
				// Bu yönlendirmenin tipini değiştir ki handle_redirections
				// onu normal bir '<' gibi işlesin.
				redir->type = TOKEN_REDIR_IN;
			}
			redir_list = redir_list->next;
		}
		// Döngü bittiğinde, eğer last_heredoc_filename hala NULL değilse
		// (listede hiç heredoc yoksa veya bir hata olduysa), temizle.
		if (last_heredoc_filename)
			free(last_heredoc_filename);

		cmd_chain = cmd_chain->next;
	}
	return (0);
}