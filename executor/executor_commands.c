/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:55:03 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/08 09:40:15 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * execute_builtin - Gelen komutun bir yerleşik komut olup olmadığını
 *                   kontrol eder ve eğer öyleyse ilgili fonksiyonu çağırır.
 *
 * Döner Değer: Komut built-in ise 1, değilse 0.
 */
int	is_builtin(char *cmd)
{
	// Koruma: Eğer komut string'i NULL ise, bu bir builtin olamaz.
	if (!cmd)
		return (0);

	// Bilinen builtin komut listesi ile karşılaştır.
	if (strcmp(cmd, "echo") == 0)
		return (1);
	if (strcmp(cmd, "cd") == 0)
		return (1);
	if (strcmp(cmd, "pwd") == 0)
		return (1);
	if (strcmp(cmd, "export") == 0)
		return (1);
	if (strcmp(cmd, "unset") == 0)
		return (1);
	if (strcmp(cmd, "env") == 0)
		return (1);
	if (strcmp(cmd, "exit") == 0)
		return (1);

	// Eğer listede bulunamadıysa, bu bir builtin değildir.
	return (0);
}

int	execute_builtin(char **args, t_shell *shell)
{
	int return_status;
	
	// Koruma: Bu durumun oluşmaması gerekir ama güvenlik için iyidir.
	if (!args || !args[0])
		return (1); // Genel hata kodu

	if (strcmp(args[0], "echo") == 0)
	{
		return (ft_echo(args, shell));
	}
	else if (strcmp(args[0], "cd") == 0)
		return (ft_cd(args, shell));
	else if (strcmp(args[0], "pwd") == 0)
		return (ft_pwd(args, shell));
	else if (strcmp(args[0], "export") == 0)
		return (ft_export(args, shell));
	else if (strcmp(args[0], "unset") == 0)
		return (ft_unset(args, shell));
	else if (strcmp(args[0], "env") == 0)
		return (ft_env(args, shell));
	else if (strcmp(args[0], "exit") == 0)
	{
		return_status = ft_exit(args, shell);

		// Eğer ft_exit "çok fazla argüman" hatası verdiyse (1 döndürdüyse),
		// bu komutun çıkış kodu 1'dir.
		if (return_status != SHELL_SHOULD_EXIT)
			shell->exit_code = return_status;
		
		// AKIŞ KONTROL SİNYALİNİ yukarıya aynen ilet.
		return (return_status);
	}

	// Bu kısma asla ulaşılmamalıdır, çünkü 'is_builtin' önceden kontrol eder.
	// Bir fallback olarak "command not found" döndürmek güvenli bir yaklaşımdır.
	return (127);
}

static void	child_cleanup_and_exit(t_shell *shell, int exit_code)
{
	// 1. Ana kabuk yapısının kaynaklarını temizle (en önemlisi env_list).
	if (shell)
	{
		// Not: ft_lstclear(&shell->env_list, free_env_content); gibi bir çağrı
		// main'deki cleanup_shell fonksiyonunu taklit etmelidir.
		// Eğer genel bir `free_shell(shell)` fonksiyonunuz yoksa,
		// en azından env_list'i temizlemek önemlidir.
		ft_lstclear(&shell->env_list, free_env_content);
	}

	// 2. Readline geçmişini temizle.
	rl_clear_history();

	// 3. Belirtilen kod ile çıkış yap.
	exit(exit_code);
}

/**
 * execute_external_in_child - Bir harici komutu `execve` ile çalıştırır.
 *
 * Bu fonksiyon, zaten `fork` ile oluşturulmuş bir çocuk sürecin
 * içinden çağrılmak üzere tasarlanmıştır.
 *
 * Başarılı olursa asla geri dönmez. Başarısız olursa hata basar ve çıkar.
 */
void	execute_external_in_child(char **args, t_shell *shell)
{
	char	*cmd_path;
	char	**envp_array;

	if (!args || !args[0] || !*args[0])
		cleanup_and_exit(shell, 0); // Basit exit(0) DEĞİL!

	cmd_path = get_command_path(args[0], shell);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cleanup_and_exit(shell, 127); // Basit exit(127) DEĞİL!
	}
	envp_array = convert_env_list_to_array(shell);
	if (!envp_array)
	{
		free(cmd_path);
		cleanup_and_exit(shell, EXIT_FAILURE); // Basit exit(1) DEĞİL!
	}
	execve(cmd_path, args, envp_array);
	
	// execve sadece hata durumunda geri döner
	perror(args[0]);
	free(cmd_path);
	ft_free_array(envp_array);
	cleanup_and_exit(shell, 126); // Basit exit(126) DEĞİL!
}

