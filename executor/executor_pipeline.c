/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:48:26 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 18:23:51 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Bir çocuk sürecin I/O yönlendirmelerini ayarlar.
 * 
 * @param current_cmd Mevcut komut.
 * @param pipe_fds Mevcut komut için oluşturulan pipe.
 * @param input_fd Bir önceki komuttan gelen pipe'ın okuma ucu.
 */
static void	setup_child_io(t_command_chain *current_cmd, int pipe_fds[2],
														int input_fd)
{
	// 1. GİRDİ YÖNLENDİRMESİ
	// Eğer bu, zincirin ilk komutu değilse (`input_fd` standart girdiden farklıysa),
	// standart girdimizi bir önceki pipe'ın okuma ucuna bağla.
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd); // Artık orijinaline ihtiyaç yok.
	}

	// 2. ÇIKTI YÖNLENDİRMESİ
	// Eğer bu, zincirin son komutu değilse,
	// standart çıktımızı mevcut pipe'ın yazma ucuna bağla.
	if (current_cmd->next)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);
	}

	// 3. TÜM PIPE'LARI KAPAT
	// Çocuk süreç, ihtiyaç duyduğu pipe'ları kopyaladıktan sonra
	// orijinal pipe tanımlayıcılarına artık ihtiyaç duymaz. Bunları kapatmak,
	// EOF sinyalinin doğru iletilmesi için hayati önem taşır.
	if (current_cmd->next)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
}

/**
 * @brief Her bir çocuk sürecin çalıştıracağı ana mantık.
 * 
 * Bu fonksiyon, G/Ç yönlendirmelerini ayarlar ve komutu çalıştırır.
 * Başarılı olursa `exit` ile sonlanır, asla geri dönmez.
 */
static void	child_routine(t_simple_command *cmd, t_shell *shell)
{
	int	exit_code;
	// Mevcut handle_redirections fonksiyonunuzu çağırmak için
	// geçici bir dizi tanımlayın. Bu dizi burada kullanılmayacak
	// ama fonksiyonun imzası bunu gerektiriyor.
	int	temp_original_fds[2];

	// 1. ADIM: Mevcut yönlendirme fonksiyonunuzu çağırın.
	if (handle_redirections(cmd, temp_original_fds) == -1)
	{
		// Yönlendirme hatası (örn: dosya açılamadı), 1 koduyla çık.
		cleanup_and_exit(shell, 1);
	}

	// 2. ADIM: Komutun kendisini çalıştır.
	if (cmd && cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		exit_code = execute_builtin(cmd->args, shell);
		cleanup_and_exit(shell, exit_code);
	}
	else
	{
		// Bu fonksiyon zaten boş komutları kontrol edip 0 ile çıkıyor.
		execute_external_in_child(cmd->args, shell);
	}
}

/* ========================================================================== */
/*                         ANA PİPELİNE FONKSİYONU                            */
/* ========================================================================== */

/**
 * @brief Oluşturulan tüm çocuk süreçlerin bitmesini bekler.
 * 
 * Bu fonksiyon, zombi süreçleri önlemek için kritik öneme sahiptir.
 * Önce son komutu bekleyerek $? durum kodunu ayarlar, sonra
 * geriye kalan tüm süreçleri temizler.
 *
 * @param last_pid Zincirdeki son komutun proses ID'si.
 * @param shell Kabuğun genel durumu ($? güncellemesi için).
 */
static void	wait_for_all_children(pid_t last_pid, t_shell *shell)
{
	int	status;
	int	printed_signal_message; // Sinyal mesajını sadece bir kez basmak için

	printed_signal_message = 0;

	// 1. Önce SON komutun bitmesini bekle.
	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
			{
				write(STDOUT_FILENO, "\n", 1);
				printed_signal_message = 1; // Mesaj basıldı olarak işaretle
			}
			// Sinyal mesajını burada bas.
			if (WTERMSIG(status) == SIGQUIT)
			{
				ft_putstr_fd("Quit (core dumped)\n", 2);
				printed_signal_message = 1; // Mesaj basıldı olarak işaretle
			}
			shell->exit_code = 128 + WTERMSIG(status);
		}
	}

	// 2. Geriye kalan diğer TÜM çocuk süreçlerin bitmesini bekle.
	while (wait(&status) > 0)
	{
		// Eğer ilk beklediğimiz çocuk bir sinyalle ölürse ve bu `last_pid` değilse,
		// yine de sinyal mesajını bir kez basmalıyız.
		if (!printed_signal_message && WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
			{
				if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				if (WTERMSIG(status) == SIGQUIT)
					ft_putstr_fd("Quit (core dumped)\n", 2);
				
				printed_signal_message = 1; // Sadece ilk ölen çocuğun mesajını bas.
			}
		}
	}
}

/**
 * @brief Komut zincirini `pipe`'lar aracılığıyla çalıştırır.
 *
 * Bu fonksiyon, bir "daisy chain" (papatya zinciri) mantığıyla çalışır:
 * - Döngüdeki her komut için bir `pipe` ve bir `fork` oluşturur.
 * - Ebeveyn süreç (parent), pipe'ları yönetir ve bir sonraki komuta
 *   doğru girdiyi aktarır.
 * - Çocuk süreç (child), I/O'sunu ayarlar ve komutu çalıştırır.
 * - En sonda, ebeveyn tüm çocukların bitmesini bekler.
 */
void	execute_pipeline(t_command_chain *cmd_chain, t_shell *shell)
{
	int		pipe_fds[2];
	int		input_fd;
	pid_t	last_pid;
	t_command_chain *current_cmd;

	input_fd = STDIN_FILENO; // İlk komutun girdisi standart girdidir.
	last_pid = -1; // Henüz bir çocuk süreç yok.
	current_cmd = cmd_chain;

	while (current_cmd)
	{
		// Eğer bu son komut değilse, bir sonraki komut için bir pipe oluştur.
		if (current_cmd->next)
			if (pipe(pipe_fds) == -1)
				return (perror("minishell: pipe"));

		last_pid = fork();
		if (last_pid == -1)
			return (perror("minishell: fork"));
		
		if (last_pid == 0) // --- ÇOCUK SÜREÇ ---
		{
			// 1. ADIM: Sinyal yöneticilerini sıfırla.
			// Pipeline'daki her bir çocuk, sinyallere bağımsız olarak
			// tepki verebilmelidir.
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			
			setup_child_io(current_cmd, pipe_fds, input_fd);
			child_routine(current_cmd->simple_command, shell);
		}
		
		// --- EBEVEYN SÜREÇ ---
		// Ebeveyn, bir önceki pipe'ın okuma ucunu artık kullanmayacağı için kapatır.
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		
		// Eğer bir sonraki komut varsa, ebeveyn şunları yapar:
		if (current_cmd->next)
		{
			// 1. Yeni pipe'ın yazma ucunu kapatır. Buna sadece çocuk ihtiyaç duyar.
			close(pipe_fds[1]);
			// 2. Bir sonraki döngü için girdi olarak bu pipe'ın okuma ucunu ayarlar.
			input_fd = pipe_fds[0];
		}
		current_cmd = current_cmd->next;
	}
	// Tüm komutlar forkladıktan sonra, hepsinin bitmesini bekle.
	wait_for_all_children(last_pid, shell);
}
