/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:00:19 by nuciftci          #+#    #+#             */
/*   Updated: 2025/07/27 18:00:22 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static bool	is_n_flag(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (false);
	i = 2;
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0'); // Sadece -n, -nnn... geçerli
}

int	ft_echo(char **args)
{
	int		i = 1;
	bool	newline = true;

	// Bir veya daha fazla -n bayrağını atla
	while (args[i] && is_n_flag(args[i]))
	{
		newline = false;
		i++;
	}

	// Argümanları boşlukla yazdır
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}

	// Satır sonu
	if (newline)
		printf("\n");
	return (0);
}
