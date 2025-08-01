/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:26:50 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/01 07:44:33 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * execute_builtin - Komutun bir yerleşik komut olup olmadığını kontrol eder
 *                   ve eğer öyleyse ilgili fonksiyonu çağırır.
 *
 * Bu fonksiyon, uzun bir if-else zinciri kullanarak komut ismini
 * bildiğimiz yerleşik komutlarla karşılaştırır.
 *
 * Parametreler:
 *   args: Komut ve argümanlarını içeren dizi.
 *   shell: Ana kabuk yapısı.
 *
 * Döner Değer:
 *   - Eğer komut bir built-in ise ve çalıştırıldıysa 1.
 *   - Eğer komut bir built-in değilse 0.
 */
static int	execute_builtin(char **args, t_shell *shell)
{
	if (args[0] == NULL) // Boş komut gelirse
		return (0);

	if (strcmp(args[0], "echo") == 0)
		shell->exit_code = ft_echo(args);
	else if (strcmp(args[0], "cd") == 0)
		shell->exit_code = ft_cd(args, shell);
	else if (strcmp(args[0], "pwd") == 0)
		shell->exit_code = ft_pwd(args);
	else if (strcmp(args[0], "export") == 0)
		shell->exit_code = ft_export(args, shell);
	else if (strcmp(args[0], "unset") == 0)
		shell->exit_code = ft_unset(args, shell);
	else if (strcmp(args[0], "env") == 0)
		shell->exit_code = ft_env(args, shell);
	else if (strcmp(args[0], "exit") == 0)
		ft_exit(args); // exit özel, çünkü programı sonlandırır.
	else
	{
		// Bu bir yerleşik komut değil.
		return (0);
	}
	// Eğer buraya geldiysek, bir yerleşik komut çalıştırdık demektir.
	return (1);
}

/**
 * executor - Komut ağacını alır ve her bir komutu çalıştırır.
 *
 * Şimdilik sadece tek bir komutu (pipe'sız) ele alacak şekilde
 * basit bir iskelet oluşturuyoruz.
 *
 * Adımlar:
 * 1. Gelen komut zincirinin geçerli olup olmadığını kontrol eder.
 * 2. İlk komutun argümanlarını alır.
 * 3. Bu komutu çalıştırması için `execute_builtin`'i çağırır.
 * 4. Eğer `execute_builtin` komutun bir built-in olmadığını söylerse (0 dönerse),
 *    o zaman bu komutun harici bir program olduğunu anlar ve onu
 *    çalıştırması için `execute_external`'ı (ileride yazacağız) çağırır.
 */
void	executor(t_command_chain *cmd_chain, t_shell *shell)
{
	char	**args;

	// Eğer parser'dan komut gelmediyse veya komut boşsa, bir şey yapma.
	if (cmd_chain == NULL || cmd_chain->simple_command == NULL)
		return;
	
	// Şimdilik sadece ilk komutu ele alıyoruz.
	args = cmd_chain->simple_command->args;
	if (args == NULL || args[0] == NULL)
		return;

	// Adım 1: Bu bir yerleşik komut mu diye kontrol et.
	if (execute_builtin(args, shell) == 1)
	{
		// Evet, bir built-in çalıştırıldı. Executor'ın işi bitti.
		return;
	}

	// Adım 2: Eğer built-in değilse, bu harici bir komuttur.
	// execute_external(args, shell); // BİR SONRAKİ ADIMIMIZ BU OLACAK!
	printf("minishell: %s: command not found\n", args[0]); // Şimdilik hata basalım.
	shell->exit_code = 127;
}