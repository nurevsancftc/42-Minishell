/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:58:57 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/08 08:54:18 by nuciftci         ###   ########.fr       */
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
	char	*temp_filename;

	while (cmd_chain)
	{
		redir_list = cmd_chain->simple_command->redirections;
		while (redir_list)
		{
			redir = (t_redir *)redir_list->content;
			if (redir->type == TOKEN_HEREDOC)
			{
				// `read_heredoc_to_temp_file` çağrılıyor...
				temp_filename = read_heredoc_to_temp_file(redir, shell);
				if (!temp_filename)
					return (-1); // Hata veya Ctrl+D

				// --- İŞTE SIZINTIYI KAPATAN KISIM ---
				// `parser`'da `ft_strunquote` ile ayrılan orijinal
				// sonlandırıcı string'ini şimdi serbest bırakıyoruz.
				free(redir->filename);
				
				// Ve yerine geçici dosyanın adını atıyoruz.
				redir->filename = temp_filename;

				// Bu heredoc artık bir girdi yönlendirmesi gibi davranacak.
				redir->type = TOKEN_REDIR_IN;
			}
			redir_list = redir_list->next;
		}
		cmd_chain = cmd_chain->next;
	}
	return (0);
}
