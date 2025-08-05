/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:55:03 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/05 13:47:01 by aldurmaz         ###   ########.fr       */
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
		// ft_exit özel bir durumdur, programı sonlandırır ve geri dönmez.
		ft_exit(args, shell);
		// Bu satıra normalde ulaşılmaz.
		return (0);
	}

	// Bu kısma asla ulaşılmamalıdır, çünkü 'is_builtin' önceden kontrol eder.
	// Bir fallback olarak "command not found" döndürmek güvenli bir yaklaşımdır.
	return (127);
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

	cmd_path = get_command_path(args[0], shell);
	if (!cmd_path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", args[0]);
		// Bellek temizliği (gerekirse)
		exit(127);
	}
	envp_array = convert_env_list_to_array(shell);
	execve(cmd_path, args, envp_array);
	perror("minishell");
	free(cmd_path);
	ft_free_array(envp_array);
	exit(126);
}

