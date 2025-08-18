/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:58:57 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/18 19:30:00 by aldurmaz         ###   ########.fr       */
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
static int	read_heredoc_input(t_redir *redir, int fd, t_shell *shell)
{
	char	*line;
	char	*expanded_line;

	// 1. SİNYAL YÖNETİCİSİNİ HEREDOC MODUNA GEÇİR
	signal(SIGINT, heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN); // Heredoc sırasında Ctrl+\ de çalışmamalı

	while (1)
	{
		line = readline("> ");
		
		//`Ctrl+C` basıldı mı diye KONTROL ET.
		if (g_status == STATUS_HEREDOC_CTRL_C)
		{
			free(line); // `readline` NULL dönmüş olabilir, yine de free güvenlidir.
			return (-1); // Hata/İptal durumu
		}
		if (!line) // `Ctrl+D` basıldı.
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(redir->filename, 2);
			ft_putstr_fd("')\n", 2);
			break;
		}
		if (strcmp(line, redir->filename) == 0)
		{
			free(line);
			break;
		}
		
		if (redir->expand_in_heredoc)
		{
			expanded_line = expand_heredoc_line(line, shell);
			write(fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
			write(fd, line, ft_strlen(line));
		
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}


/**
 * handle_heredocs - Tüm komut zincirindeki `heredoc`'ları işler.
 */
int	handle_heredocs(t_command_chain *cmd_chain, t_shell *shell)
{
	t_list	*redir_list;
	t_redir	*redir;
	char	*tmp_filename;
	int		fd;

	while (cmd_chain)
	{
		redir_list = cmd_chain->simple_command->redirections;
		while (redir_list)
		{
			redir = (t_redir *)redir_list->content;
			if (redir->type == TOKEN_HEREDOC)
			{
				// Okumaya başlamadan önce sinyal durumunu sıfırla.
				g_status = STATUS_OK;
								
				tmp_filename = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
				fd = mkstemp(tmp_filename);
				if (fd < 0)
					return (free(tmp_filename), -1);

				if (read_heredoc_input(redir, fd, shell) == -1)
				{
					// Okuma Ctrl+C ile kesildi.
					close(fd);
					unlink(tmp_filename); // Oluşturulan geçici dosyayı sil.
					free(tmp_filename);
					shell->exit_code = 130;
					setup_interactive_signals(); // Sinyalleri normale döndür.
					return (-1); // Ana executor'a hata bildir.
				}
				close(fd);
				
				free(redir->filename);
				redir->filename = tmp_filename;
				redir->type = TOKEN_REDIR_IN;
			}
			redir_list = redir_list->next;
		}
		cmd_chain = cmd_chain->next;
	}
	
	// Her şey bittikten sonra, sinyalleri normale döndür.
	setup_interactive_signals();
	return (0);
}
