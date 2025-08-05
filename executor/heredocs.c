/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:58:57 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/05 19:59:08 by aldurmaz         ###   ########.fr       */
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
static char	*read_heredoc_to_temp_file(t_redir *redir, t_shell *shell)
{
	char	*line;
	char	*expanded_line;
	int		fd;
	char	*tmp_filename;

	tmp_filename = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
	if (!tmp_filename)
		return (NULL);
	fd = mkstemp(tmp_filename);
	if (fd < 0)
		return (free(tmp_filename), NULL);

	while (1)
	{
		line = readline("> ");
		if (!line) // Ctrl+D
			break;
		if (strcmp(line, redir->filename) == 0) // redir->filename artık temiz delimiter'ı tutuyor
		{
			free(line);
			break;
		}
		
		// GENİŞLETME MANTIĞI BURADA
		if (redir->expand_in_heredoc)
		{
			// Evet, genişletme gerekiyor.
			expanded_line = expand_heredoc_line(line, shell);
			write(fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
		{
			// Hayır, genişletme yok. Olduğu gibi yaz.
			write(fd, line, ft_strlen(line));
		}
		
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
int	handle_heredocs(t_command_chain *cmd_chain, t_shell *shell)
{
	t_list	*redir_list;
	t_redir	*redir;
	t_list	*last_heredoc_node;

	while (cmd_chain)
	{
		// 1. ADIM: Son heredoc düğümünü bul.
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
				// `read_heredoc_to_temp_file` artık t_redir'in tamamını ve shell'i alıyor.
				if (redir_list != last_heredoc_node) // Bu sonuncu değil mi?
				{
					char *dummy_file = read_heredoc_to_temp_file(redir, shell);
					if (!dummy_file) return (-1);
					unlink(dummy_file);
					free(dummy_file);
					redir->filename = NULL; // filename'i free etmiyoruz, çünkü parser'da temizlendi.
				}
				else // Bu, son ve geçerli heredoc.
				{
					char *real_file = read_heredoc_to_temp_file(redir, shell);
					if (!real_file) return (-1);
					// Delimiter'ın yerini geçici dosya adı aldı.
					// Orijinal delimiter'ı free etmeye gerek yok, parser bunu yaptı.
					redir->filename = real_file;
					redir->type = TOKEN_REDIR_IN;
				}
			}
			redir_list = redir_list->next;
		}
		cmd_chain = cmd_chain->next;
	}
	return (0);
}
