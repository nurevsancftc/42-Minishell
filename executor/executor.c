/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:26:50 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/08 08:51:00 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * executor - Komut ağacını yürüten ana fonksiyon.
 *
 * 1. Heredoc'ları işler: Komutlar çalışmadan önce kullanıcıdan girdi alır.
 * 2. Expander'ı çalıştırır: Değişkenleri ($USER gibi) genişletir ve tırnakları kaldırır.
 * 3. Komutun tek mi yoksa bir pipeline mı olduğuna karar verir ve ilgili
 *    yürütme fonksiyonunu çağırır.
 */
void	executor(t_command_chain *cmd_chain, t_shell *shell)
{
	if (!cmd_chain)
		return;

	// 1. ADIM: Heredoc'ları ÖNCEDEN işle.
	// Bu fonksiyon, zincirdeki tüm `<<`'ları bulur ve geçici dosyalara
	// veya pipe'lara yazar. Hata olursa (örn. Ctrl+D ile kesilirse) -1 döner.
	if (handle_heredocs(cmd_chain, shell) == -1)
	{
		shell->exit_code = 1; // veya başka bir hata kodu
		return;
	}

	// 2. ADIM: Genişletme ve Tırnak Kaldırma (Expander)
	// Bu fonksiyon, komut ağacındaki tüm argümanları gezer,
	// $VAR gibi ifadeleri değerleriyle değiştirir ve "echo" gibi tırnakları kaldırır.
	// expander(cmd_chain, shell);

	// 3. ADIM: Yürütme Mantığı
	// Komut zincirinde tek bir komut mu var, yoksa pipe'larla bağlı birden fazla mı?
	if (!cmd_chain->next)
	{
		// Durum: Tek komut (örn: `ls -l > file` veya `cd ..`)
		execute_single_command(cmd_chain, shell);
	}
	else
	{
		// Durum: Pipeline (örn: `cat file | grep "test" | wc -l`)
		execute_pipeline(cmd_chain, shell);
	}
}

/**
 * execute_single_command - Yönlendirmeleri olan veya olmayan tek bir komutu çalıştırır.
 */
void	execute_single_command(t_command_chain *chain_node, t_shell *shell)
{
	int	original_fds[2];
	int	exit_status;
	t_simple_command *cmd; // Kolay erişim için bir pointer oluşturalım.

	// Koruma ve kolay erişim
	if (!chain_node || !chain_node->simple_command)
		return;
	cmd = chain_node->simple_command;

	if (!cmd->args || !cmd->args[0])
		return;

	// Yönlendirmeleri uygula
	if (handle_redirections(cmd, original_fds) == -1)
	{
		// ... (hata yönetimi) ...
		return;
	}

	if (is_builtin(cmd->args[0]))
	{
		shell->exit_code = execute_builtin(cmd->args, shell);
	}
	else
	{
		// ARADIĞINIZ CEVAP BURADA:
		// Artık hem `cmd` (simple_command) hem de `chain_node` (tüm zincir) elimizde.
		// Dolayısıyla `execute_external_command`'a her ikisini de gönderebiliriz.
		exit_status = execute_external_command(cmd, shell, chain_node);
		shell->exit_code = exit_status;
	}

	// Orijinal fd'leri geri yükle
	restore_fds(original_fds);
}




/**
 * execute_external_command - Fork yaparak harici bir komut çalıştırır.
 *
 * Bu fonksiyon, bir çocuk proses oluşturur. Çocuk proses içinde:
 * 1. Komutun çalıştırılabilir yolunu bulur.
 * 2. Ortam değişkenleri listesini (t_list) execve'nin beklediği (char**)
 *    formata dönüştürür.
 * 3. execve ile komutu çalıştırmaya çalışır.
 *
 * Herhangi bir hata durumunda (komut bulunamazsa, bellek ayrılamazsa,
 * execve başarısız olursa), çocuk proses sorumlu olduğu tüm belleği
 * temizleyerek programdan çıkar.
 *
 * Ebeveyn proses, çocuğun bitmesini bekler ve çıkış kodunu döndürür.
 *
 * @param cmd: Çalıştırılacak olan basit komut yapısı.
 * @param shell: Ortam değişkenlerine erişim için ana shell yapısı.
 * @param full_chain: Hata durumunda tüm komut ağacını temizlemek için
 *                    zincirin başlangıcı.
 * @return: Komutun çıkış kodunu döndürür.
 */
int	execute_external_command(t_simple_command *cmd, t_shell *shell, \
							t_command_chain *full_chain)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1); // Fork hatası
	if (pid == 0)
	{
		// --- Çocuk Proses Başlangıcı ---
		char **envp_array;

		path = get_command_path(cmd->args[0], shell);
		if (!path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			return (SHELL_SHOULD_EXIT);
		}

		envp_array = convert_env_list_to_array(shell);
		if (!envp_array)
		{
			perror("minishell: environment setup failed");
			free(path); // `get_command_path`'ten gelen path'i unutma.
			return (SHELL_SHOULD_EXIT);
		}

		execve(path, cmd->args, envp_array);

		// Bu noktaya sadece execve başarısız olursa ulaşılır.
		perror(cmd->args[0]);
		free(path);
		ft_free_array(envp_array); // execve'ye gönderdiğimiz diziyi temizle.
		return (SHELL_SHOULD_EXIT);
	}

	// --- Ebeveyn Proses Başlangıcı ---
	waitpid(pid, &status, 0);

	// Çocuğun çıkış durumunu analiz et ve uygun kodu döndür.
	if (WIFEXITED(status))
		return (WEXITSTATUS(status)); // Normal şekilde sonlandı.
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128); // Sinyal ile sonlandırıldı (örn: Ctrl+C).
	
	return (1); // Beklenmedik bir durum.
}
