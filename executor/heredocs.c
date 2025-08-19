/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:58:57 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 22:59:00 by aldurmaz         ###   ########.fr       */
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
/**
 * @brief Bu fonksiyon SADECE çocuk süreç tarafından çalıştırılır.
 *
 * Kullanıcıdan girdi okur ve heredoc içeriğini geçici dosyaya yazar.
 * Başarılı olursa 0 ile çıkar, hata olursa 1 ile çıkar.
 * Ctrl+C sinyali bu süreci direkt olarak sonlandırır.
 */
static void	child_heredoc_routine(t_redir *redir, int fd, t_shell *shell)
{
	char	*line;
	char	*expanded_line;

	// 1. ADIM: Bu çocuk sürecin sinyal davranışını ayarla.
	// SIGINT (Ctrl+C) varsayılan davranışa (terminate) ayarlanır.
	setup_signals(MODE_CHILD);

	while (1)
	{
		line = readline("> ");
		if (!line) // Ctrl+D basıldı.
		{
			ft_putstr_fd("minishell: warning: heredoc delimited by EOF (wanted `", 2);
			ft_putstr_fd(redir->filename, 2); // delimiter'ı bas
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
	// Çocuk işini bitirdi. Tüm belleği temizleyip çıkmalı.
	cleanup_and_exit(shell, 0); // Başarılı çıkış
}

/**
 * @brief Ebeveyn sürecin bir heredoc'u yönetmesini sağlayan fonksiyon.
 *
 * Fork oluşturur, çocuğun bitmesini bekler ve çıkış durumunu işler.
 * @return 0 başarı durumunda, -1 Ctrl+C ile iptal durumunda.
 */
static int	process_single_heredoc(t_redir *redir, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		fd;
	char	*tmp_filename; // Geçici dosya adını tutacak yerel değişken.

	// Geçici dosya adını ve dosya tanımlayıcısını hazırla.
	tmp_filename = ft_strdup("/tmp/minishell_heredoc_XXXXXX");

	if(!tmp_filename)
		return (-1); // Bellek hatası
	fd = mkstemp(tmp_filename);
	if (fd < 0)
	{
		free(tmp_filename);
		return (-1); // dosya oluşturma hatası
	}
	// 1. EBEVEYN: Çocuğu beklemeden önce SIGINT'i görmezden gel.
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		// Fork hatası durumunda kaynakları temizle
		free(tmp_filename);
		close(fd);
		return (-1);
	}
	if (pid == 0) // --- ÇOCUK SÜREÇ ---
	{
		// close(fd); // Çocuk sadece rutin içinde fd kullanacak, kopyasını değil.
		child_heredoc_routine(redir, fd, shell);
	}

	// --- EBEVEYN SÜREÇ ---
	close(fd); // Ebeveynin geçici dosyanın fd'sine ihtiyacı yok.
	
	waitpid(pid, &status, 0);
	// 2. EBEVEYN: Bekleme bitti, sinyalleri normale döndür.
	setup_signals(MODE_INTERACTIVE);
	// 3. EBEVEYN: Çocuğun nasıl sonlandığını kontrol et.
	// 4. Çocuğun çıkış durumunu kontrol et.
	if (WIFSIGNALED(status)) // Önce bir sinyalle mi öldü diye genel kontrol yap
	{
		if (WTERMSIG(status) == SIGINT) // Sinyal SIGINT miydi?
		{
			write(STDOUT_FILENO, "\n", 1);
			unlink(tmp_filename);
			free(tmp_filename);
			shell->exit_code = 130; // 128 + 2
			return (-1);
		}
		else if (WTERMSIG(status) == SIGQUIT) // <-- BU ELSE IF BLOĞUNU EKLE
		{
			// Terminal zaten "Quit (core dumped)" yazar, bizim bir şey
			// yazmamıza gerek yok, ama bash gibi davranmak için yazabiliriz.
			ft_putstr_fd("Quit (core dumped)\n", 2);
			unlink(tmp_filename);
			free(tmp_filename);
			shell->exit_code = 131; // 128 + 3
			return (-1);
		}
	}

	// 5. BAŞARILI DURUM: `t_redir` struct'ını güncelle.
	free(redir->filename);            // Eski delimiter'ı serbest bırak.
	redir->filename = tmp_filename;   // Yeni geçici dosya adını ata.
	// redir->type = TOKEN_REDIR_IN;     // Yönlendirme tipini '<' olarak değiştir.

	return (0); // Başarılı.
}

/**
 * @brief Tüm komut zincirindeki `heredoc`'ları işler.
 *
 * Bu fonksiyon, komutlar çalıştırılmadan ÖNCE çağrılır.
 * Her bir heredoc için bir alt süreç oluşturur. Eğer herhangi biri
 * Ctrl+C ile iptal edilirse, tüm işlemi durdurur ve -1 döner.
 */
int	handle_heredocs(t_command_chain *cmd_chain, t_shell *shell)
{
	t_list	*redir_list;
	t_redir	*redir;

	while (cmd_chain)
	{
		redir_list = cmd_chain->simple_command->redirections;
		while (redir_list)
		{
			redir = (t_redir *)redir_list->content;
			if (redir->type == TOKEN_HEREDOC)
			{
				if (process_single_heredoc(redir, shell) == -1)
				{
					// Bir heredoc Ctrl+C ile iptal edildi.
					// Tüm işlemi durdur ve executor'a hata bildir.
					return (-1);
				}
				// Başarılı durumda `redir` struct'ı zaten güncellendi.
				// Döngüye devam et.
			}
			redir_list = redir_list->next;
		}
		cmd_chain = cmd_chain->next;
	}
	return (0); // Tüm heredoc'lar başarıyla işlendi.
}
