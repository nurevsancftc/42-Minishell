/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:55:03 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/03 10:21:31 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * execute_builtin - Gelen komutun bir yerleşik komut olup olmadığını
 *                   kontrol eder ve eğer öyleyse ilgili fonksiyonu çağırır.
 *
 * Döner Değer: Komut built-in ise 1, değilse 0.
 */
int	execute_builtin(char **args, t_shell *shell)
{
	if (strcmp(args[0], "echo") == 0)
		// DÜZELTME: İkinci parametre olarak 'shell'i ekle.
		shell->exit_code = ft_echo(args, shell);
	else if (strcmp(args[0], "cd") == 0)
		shell->exit_code = ft_cd(args, shell);
	else if (strcmp(args[0], "pwd") == 0)
		// DÜZELTME: İkinci parametre olarak 'shell'i ekle.
		shell->exit_code = ft_pwd(args, shell);
	else if (strcmp(args[0], "export") == 0)
		shell->exit_code = ft_export(args, shell);
	else if (strcmp(args[0], "unset") == 0)
		shell->exit_code = ft_unset(args, shell);
	else if (strcmp(args[0], "env") == 0)
		shell->exit_code = ft_env(args, shell);
	else if (strcmp(args[0], "exit") == 0)
		ft_exit(args);
	else
		return (0);
	return (1);
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
	ft_free_split(envp_array);
	exit(126);
}

