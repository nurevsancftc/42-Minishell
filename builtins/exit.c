/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:45:07 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/13 20:23:55 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>      // printf
#include <stdlib.h>     // exit, atoi
#include <string.h>     // strlen
#include <unistd.h>     // write
#include "minishell.h"

static long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		// TAŞMA KONTROLÜ (LLONG_MAX ve LLONG_MIN ile)
		if (result > LLONG_MAX / 10 || \
			(result == LLONG_MAX / 10 && (str[i] - '0') > LLONG_MAX % 10))
		{
			if (sign == 1)
				return (LLONG_MAX);
			else
				return (LLONG_MIN);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}


/**
 * is_numeric - Bir string'in tamamen sayılardan oluşup oluşmadığını kontrol eder.
 *
 * Bu bir yardımcı fonksiyondur. 'ft_exit' içindeki kodu temiz tutar.
 * Baştaki '+' veya '-' işaretlerine izin verir, ama "++123" gibi şeyleri reddeder.
 */
static int	is_numeric(const char *str)
{
	int i = 0;

	// Eğer string'in başında + veya - varsa, bu karakteri atlayıp devam et.
	if (str[i] == '+' || str[i] == '-')
	{
		i++;
	}

	// Eğer string boşsa veya sadece "+" gibi bir işaretten oluşuyorsa,
	// bu sayısal bir argüman değildir.
	if (str[i] == '\0')
		return (0);

	// Geriye kalan tüm karakterlerin rakam olup olmadığını kontrol et.
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
		{
			// Rakam olmayan ilk karakteri bulduğumuz an "sayısal değil" diyoruz.
			return (0);
		}
		i++;
	}
	// Eğer döngü sonuna kadar geldiysek, tüm karakterler rakammış demektir.
	return (1);
}

/**
 * ft_exit - 'exit' yerleşik komutunu çalıştırır.
 *
 * Kabuğu sonlandırır ve bir çıkış kodu belirler. bash davranışını taklit eder:
 * - exit: Son komutun çıkış koduyla çıkar.
 * - exit [n]: n koduyla çıkar (0-255 arası).
 * - exit [sayısal olmayan]: Hata basar ve 255 ile çıkar.
 * - exit [n] [fazlası]: Hata basar ama ÇIKMAZ.
 */
int	ft_exit(char **args, t_shell *shell)
{
	long long	num;

	write(1, "exit\n", 5);
	if (args[1] == NULL)
	{
		// shell->exit_code zaten doğru değeri tutuyor. Sadece çıkış sinyali ver.
		return (SHELL_SHOULD_EXIT);
	}
	num = ft_atoll(args[1]);
	if (is_numeric(args[1]) == 0 || (num == LLONG_MAX || num == LLONG_MIN))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		shell->exit_code = 255; // Gerçek çıkış kodunu ayarla
		return (SHELL_SHOULD_EXIT); // Çıkış sinyalini ver
	}
	if (args[2] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1); // Hata ver, çıkma.
	}
	shell->exit_code = (unsigned char)num; // Gerçek çıkış kodunu ayarla
	return (SHELL_SHOULD_EXIT); // Çıkış sinyalini ver
}
