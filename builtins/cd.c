/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:16:08 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/06 01:08:34 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

/* ========================================================================== */
/*                          ASIL CD FONKSİYONLARI                             */
/* ========================================================================== */

/**
 * get_target_path - Komut argümanlarına göre gidilecek hedef yolu belirler.
 *
 * Bu fonksiyon, sistemin `getenv`'i yerine bizim kendi `get_env_value`
 * fonksiyonumuzu kullanarak kabuğun kendi ortamıyla tutarlı çalışır.
 */
static int	get_target_path(char **args, t_shell *shell, char **path)
{
	if (args[1] == NULL || strcmp(args[1], "~") == 0)
	{
		*path = get_env_value(shell->env_list, "HOME");
		if (*path == NULL)
		{
			write(2, "minishell: cd: HOME not set\n", 28);
			return (1);
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		*path = get_env_value(shell->env_list, "OLDPWD");
		if (*path == NULL)
		{
			write(2, "minishell: cd: OLDPWD not set\n", 30);
			return (1);
		}
		printf("%s\n", *path);
	}
	else
		*path = args[1];
	return (0);
}

/**
 * ft_cd - 'cd' yerleşik komutunun ana fonksiyonu.
 *
 * Bu fonksiyon, adımları yöneten bir orkestra şefi gibidir:
 * Önce argümanları kontrol eder, sonra mevcut dizini kaydeder,
 * hedef yolu bulur, dizini değiştirir ve son olarak ortamı günceller.
 */
int	ft_cd(char **args, t_shell *shell)
{
	char	*target_path;
	char	current_pwd_buffer[PATH_MAX];

	if (args[1] && args[2])
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	if (getcwd(current_pwd_buffer, PATH_MAX) == NULL)
	{
		perror("minishell: cd: getcwd error");
		return (1);
	}
	if (get_target_path(args, shell, &target_path) != 0)
		return (1);
	if (chdir(target_path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", target_path, strerror(errno));
		return (1);
	}
	update_or_create_env(shell, "OLDPWD", current_pwd_buffer);
	if (getcwd(current_pwd_buffer, PATH_MAX) != NULL)
		update_or_create_env(shell, "PWD", current_pwd_buffer);
	return (0);
}
