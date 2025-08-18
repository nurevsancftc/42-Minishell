/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:57:35 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/18 22:03:57 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * handle_redirections - Bir komutun I/O yönlendirmelerini uygular.
 *
 * Bu fonksiyon, komutun `redirections` listesini baştan sona gezer.
 * Gördüğü her yönlendirme için ilgili dosyayı açar. Eğer aynı tipte
 * (girdi veya çıktı) bir yönlendirme daha önce işlenmişse, bir önceki
 * dosya tanımlayıcısı kapatılır ve yenisi onun yerine geçer ("sonuncu kazanır").
 *
 * Döngü bittikten sonra, en son geçerli olan girdi ve çıktı dosyaları,
 * komutun standart girdi ve çıktısı olarak ayarlanır.
 *
 * @param cmd: Yönlendirmeleri uygulanacak olan komut.
 * @param original_fds: Orijinal stdin ve stdout'un yedekleneceği dizi.
 *
 * @return: Başarılı olursa 0, dosya açma veya başka bir hata olursa -1 döner.
 */
int	handle_redirections(t_simple_command *cmd, int original_fds[2])
{
	t_list	*redir_list;
	t_redir	*redir;
	int		in_fd;
	int		out_fd;

	// Başlangıçta geçerli bir yönlendirme dosyası olmadığını belirtmek için
	// fd'leri geçersiz bir değere (-1) ayarla.
	in_fd = -1;
	out_fd = -1;

	// Olası bir hata durumunda bile geri yüklenebilmeleri için
	// orijinal dosya tanımlayıcılarını en başta yedekle.
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
	if (original_fds[0] == -1 || original_fds[1] == -1)
		return (perror("minishell: dup"), -1);

	redir_list = cmd->redirections;
	while (redir_list)
	{
		redir = (t_redir *)redir_list->content;

		if (redir->filename == NULL)
		{
			redir_list = redir_list->next;
			continue;
		}
		// --- GİRDİ YÖNLENDİRMELERİ (< ve <<) ---
		if (redir->type == TOKEN_REDIR_IN || redir->type == TOKEN_HEREDOC)
		{
			// Eğer daha önceden açılmış bir girdi dosyası varsa, onu kapat.
			// Çünkü bu yeni yönlendirme eskisini geçersiz kılacak.
			if (in_fd != -1)
				close(in_fd);
			in_fd = open(redir->filename, O_RDONLY);
			if (in_fd == -1)
			{
				perror(redir->filename); // Hata mesajı bas (örn: "No such file")
				// Diğer açık fd'yi (varsa) ve yedekleri kapatıp hata ile çık.
				if (out_fd != -1)
					close(out_fd);
				close(original_fds[0]);
				close(original_fds[1]);
				return (-1);
			}
		}
		// --- ÇIKTI YÖNLENDİRMELERİ (> ve >>) ---
		else if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_REDIR_APPEND)
		{
			// Eğer daha önceden açılmış bir çıktı dosyası varsa, onu kapat.
			if (out_fd != -1)
				close(out_fd);
			
			if (redir->type == TOKEN_REDIR_OUT) // '>' durumu: Dosyayı oluştur/sıfırla
				out_fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else // '>>' durumu: Dosyayı oluştur/sonuna ekle
				out_fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);

			if (out_fd == -1)
			{
				perror(redir->filename); // Hata mesajı bas (örn: "Permission denied")
				// Diğer açık fd'yi (varsa) ve yedekleri kapatıp hata ile çık.
				if (in_fd != -1)
					close(in_fd);
				close(original_fds[0]);
				close(original_fds[1]);
				return (-1);
			}
		}
		redir_list = redir_list->next;
	}

	// Döngü bitti. En son geçerli olan in_fd ve out_fd'yi uygula.
	// Eğer hiç girdi yönlendirmesi yoksa, in_fd hala -1'dir ve bu blok atlanır.
	if (in_fd != -1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd); // dup2 kopyasını oluşturdu, orijinal fd'ye ihtiyaç kalmadı.
	}
	// Eğer hiç çıktı yönlendirmesi yoksa, out_fd hala -1'dir ve bu blok atlanır.
	if (out_fd != -1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd); // dup2 kopyasını oluşturdu, orijinal fd'ye ihtiyaç kalmadı.
	}
	return (0); // Başarılı
}

// Orijinal fd'leri geri yükleyen fonksiyon
void restore_fds(int original_fds[2])
{
    dup2(original_fds[0], STDIN_FILENO);
    dup2(original_fds[1], STDOUT_FILENO);
    close(original_fds[0]);
    close(original_fds[1]);
}
