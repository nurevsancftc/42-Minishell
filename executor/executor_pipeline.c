/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:48:26 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/08 09:40:55 by nuciftci         ###   ########.fr       */
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

	// I/O Yönlendirmelerini uygula (eğer varsa)
	// handle_redirections(cmd, ...);

	if (cmd && cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		// Builtin komutunu çalıştır ve çıkış kodunu al.
		exit_code = execute_builtin(cmd->args, shell);
		
		// Çıkmadan önce, bu çocuğun miras aldığı TÜM belleği temizle.
		cleanup_and_exit(shell, exit_code); // Basit exit() DEĞİL!
	}
	else
	{
		// Harici komutlar için olan fonksiyon zaten kendi içinde
		// `cleanup_and_exit`'i çağırıyor.
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

	// 1. Önce SON komutun bitmesini bekle. $? onun durumuna göre ayarlanır.
	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_code = WTERMSIG(status) + 128;
	}

	// 2. Geriye kalan diğer TÜM çocuk süreçlerin bitmesini bekle.
	// Bu döngü, bekleyecek çocuk kalmadığında (wait, -1 döndürdüğünde) biter.
	// Bu, sistemde zombi süreç kalmamasını garanti eder.
	while (wait(NULL) > 0)
		;
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
