/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:26:50 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/14 18:54:36 by nuciftci         ###   ########.fr       */
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
	int					original_fds[2];
	int					exit_status;
	t_simple_command	*cmd;

	if (!chain_node || !chain_node->simple_command)
		return (0);
	cmd = chain_node->simple_command;

	// 1. ADIM: Her zaman önce yönlendirmeleri uygula.
	// Bu, `> file` gibi komutların, komut boş olsa bile çalışmasını sağlar.
	if (handle_redirections(cmd, original_fds) == -1)
	{
		shell->exit_code = 1;
		return (0); // Yönlendirme hatası, ama kabuk devam etmeli.
	}

	// 2. ADIM: "Gerçekten" boş komut mu diye kontrol et ($EMPTY, `> file` gibi).
	// Bu, `expander`'ın argüman listesini tamamen boşalttığı durumdur.
	if (!cmd->args || !cmd->args[0])
	{
		// Bu bir hata değildir. Sadece yönlendirmeler yapıldı.
		restore_fds(original_fds);
		return (0);
	}
	
	// 3. ADIM: Komutun builtin mi harici mi olduğunu kontrol et ve çalıştır.
	// NOT: `""` komutu `!cmd->args[0]` kontrolünden geçer (çünkü `args[0]`
	// NULL değildir) ve harici komut olarak işlem görür. `execute_external_command`
	// içinde "command not found" hatası alması beklenir. Bu doğrudur.
	if (is_builtin(cmd->args[0]))
	{
		exit_status = execute_builtin(cmd->args, shell);
		if (exit_status != SHELL_SHOULD_EXIT)
			shell->exit_code = exit_status;
		else
		{
			// `exit` komutu çağrıldı. Sinyali yukarıya pasla.
			restore_fds(original_fds);
			return (SHELL_SHOULD_EXIT);
		}
	}
	else
	{
		exit_status = execute_external_command(cmd, shell, chain_node);
		shell->exit_code = exit_status;
	}

	// 4. ADIM: Her şey bittikten sonra orijinal fd'leri geri yükle.
	restore_fds(original_fds);
	return (0); // Normal şekilde devam et.
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

	(void)full_chain;
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
	{
		// --- Çocuk Proses Başlangıcı ---
		char	*command;
		char	**envp;

		command = cmd->args[0];
		// `envp`'yi en başta oluştur. Her çıkış yolunda bunu temizleyeceğiz.
		envp = convert_env_list_to_array(shell);

		if (ft_strchr(command, '/'))
		{
			char	original_pwd[PATH_MAX];

			// 1. ADIM: Mevcut dizini kaydet (her ihtimale karşı).
			// getcwd başarısız olursa diye kontrol eklemek en sağlıklısı.
			if (getcwd(original_pwd, PATH_MAX) == NULL)
			{
				perror("minishell: getcwd");
				ft_free_array(envp);
				cleanup_and_exit(shell, 1);
			}

			// 2. ADIM: Dizin mi diye chdir ile test et.
			if (chdir(command) == 0)
			{
				// chdir başarılı oldu, bu bir dizin.
				chdir(original_pwd); // HEMEN GERİ DÖN!
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(command, 2);
				ft_putstr_fd(": is a directory\n", 2);
				ft_free_array(envp);
				cleanup_and_exit(shell, 126);
			}

			// 3. ADIM: Var mı diye kontrol et (ama dizin değil).
			if (access(command, F_OK) != 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(command, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				ft_free_array(envp);
				cleanup_and_exit(shell, 127);
			}
			
			// 4. ADIM: Yol var ve dizin değil, çalıştırmayı dene.
			execve(command, cmd->args, envp);
			perror(command);
			ft_free_array(envp);
			cleanup_and_exit(shell, 126);
		}
		else
		{
			char *path = get_command_path(command, shell);
			if (!path)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(command, 2);
				ft_putstr_fd(": command not found\n", 2);
				ft_free_array(envp); // <-- TEMİZLİK
				cleanup_and_exit(shell, 127);
			}
			execve(path, cmd->args, envp);
			perror(path);
			free(path);
			ft_free_array(envp); // <-- TEMİZLİK
			cleanup_and_exit(shell, 126);
		}
	}

	// --- Ebeveyn Proses Başlangıcı ---
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
}
