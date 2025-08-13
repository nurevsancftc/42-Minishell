/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:26:50 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/13 20:29:40 by nuciftci         ###   ########.fr       */
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
int	executor(t_command_chain *cmd_chain, t_shell *shell)
{
	int status;

	if (!cmd_chain)
		return (0);
	if (handle_heredocs(cmd_chain, shell) == -1)
	{
		shell->exit_code = 1;
		return (0);
	}
	// expander(cmd_chain, shell); // Expander'ı burada çağırdığınızı varsayıyorum

	if (!cmd_chain->next)
	{
		// Tek komut: execute_single_command'dan gelen sinyali yakala.
		status = execute_single_command(cmd_chain, shell);
		if (status == SHELL_SHOULD_EXIT)
			return (SHELL_SHOULD_EXIT); // Sinyali yukarı pasla
	}
	else
	{
		// Pipeline: 'exit' bir alt kabukta çalışır ve ana kabuğu
		// sonlandırmaz. Bu yüzden sinyali kontrol etmeye gerek yok.
		execute_pipeline(cmd_chain, shell);
	}
	return (0); // Normal devam et
}

/**
 * execute_single_command - Yönlendirmeleri olan veya olmayan tek bir komutu çalıştırır.
 */
int	execute_single_command(t_command_chain *chain_node, t_shell *shell)
{
	int	original_fds[2];
	int	exit_status;
	t_simple_command *cmd;

	if (!chain_node || !chain_node->simple_command)
		return (0); // Normal devam et
	cmd = chain_node->simple_command;
	if (!cmd->args || !cmd->args[0])
		return (0); // Boş komut, devam et

	if (handle_redirections(cmd, original_fds) == -1)
	{
		shell->exit_code = 1;
		return (0); // Devam et, kabuktan çıkma
	}

	if (is_builtin(cmd->args[0]))
	{
		exit_status = execute_builtin(cmd->args, shell);
		if (exit_status != SHELL_SHOULD_EXIT)
		{
			// Normal bir builtin, $?'ı güncelle
			shell->exit_code = exit_status;
		}
		else
		{
			// `exit` komutu çağrıldı. `shell->exit_code`'a dokunma.
			// Sadece çıkış sinyalini yukarıya döndür.
			restore_fds(original_fds);
			return (SHELL_SHOULD_EXIT);
		}
	}
	else
	{
		exit_status = execute_external_command(cmd, shell, chain_node);
		shell->exit_code = exit_status;
	}

	restore_fds(original_fds);
	return (0); // Normal şekilde devam et
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
