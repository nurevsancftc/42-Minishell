/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:55:03 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/01 08:55:12 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * execute_builtin - Gelen komutun bir yerleşik komut olup olmadığını
 *                   kontrol eder ve eğer öyleyse ilgili fonksiyonu çağırır.
 *
 * Bir `if-else` zinciri kullanarak komut ismini bildiğimiz yerleşik
 * komutlarla karşılaştırır. Eşleşme bulursa, ilgili `ft_` fonksiyonunu
 * çalıştırır ve kabuğun çıkış kodunu günceller.
 *
 * Döner Değer:
 *   - Komut bir built-in ise ve çalıştırıldıysa: 1
 *   - Komut bir built-in değilse: 0
 */
int	execute_builtin(char **args, t_shell *shell)
{
	// `args[0]` komutun kendisidir.
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
		ft_exit(args); // exit özeldir, geri dönmez.
	else
	{
		// Komut, bildiğimiz yerleşik komutlardan biri değil.
		return (0);
	}

	// Eğer buraya ulaştıysak, bir yerleşik komut çalıştırdık demektir.
	return (1);
}
