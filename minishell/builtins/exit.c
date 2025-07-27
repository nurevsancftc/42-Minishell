/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:45:07 by nuciftci          #+#    #+#             */
/*   Updated: 2025/07/27 19:12:26 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdio.h>      // printf
#include <stdlib.h>     // exit, atoi
#include <string.h>     // strlen
#include <unistd.h>     // write

/**
 * is_numeric - Bir string'in sayısal (integer) olup olmadığını kontrol eder.
 * İsteğe bağlı olarak başında '+' veya '-' olabilir.
 *
 * @param str Kontrol edilecek string.
 * @return Sayısal ise 1, değilse 0.
 */
static int	is_numeric(const char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str) // yalnızca '+' veya '-' varsa geçersiz
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

/**
 * ft_exit - Minishell'den çıkış işlemini yönetir.
 *
 * Desteklenen durumlar:
 * - `exit`: 0 ile çıkış
 * - `exit [num]`: num ile çıkış
 * - `exit [non-numeric]`: stderr'e hata basar, 255 ile çıkış
 * - `exit [num] [extra]`: stderr'e hata basar, çıkış yapmaz
 *
 * @param args Argümanlar dizisi (args[0] = "exit", args[1..n] = diğer argümanlar)
 * @return Eğer çıkış yapılmazsa 1 (hata); aksi takdirde `exit()` çağrılır.
 */
int	ft_exit(char **args)
{
	int	status;

	printf("exit\n");

	if (!args[1]) // exit
		exit(0);
	else if (!is_numeric(args[1])) // exit abc
	{
		write(2, "minishell: exit: ", 17);
		write(2, args[1], strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		exit(255);
	}
	else if (args[2]) // exit 42 7
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	else // exit 42
	{
		status = atoi(args[1]);
		exit((unsigned char)status);
	}
}
