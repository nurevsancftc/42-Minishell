/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:27:32 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/14 20:58:18 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * ft_dot - '.' (source) yerleşik komutunu ele alır.
 *
 * Bu komutun tam olarak uygulanması zorunlu değildir.
 * Ancak, bash uyumluluğu için en yaygın hata durumunu,
 * yani argüman olmadan çağrılmasını doğru bir şekilde ele almalıyız.
 */
int	ft_dot(char **args, t_shell *shell)
{
	int	error_status;

	(void)shell;
	if (args[1] == NULL || args[1][0] == '\0')
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		return (2);
	}

	// Hata kontrolünü ve mesaj basmayı yardımcıya devret.
	// `.` komutunun hata mesajında dosya adı görünür, komutun kendisi değil.
	error_status = ft_check_path_error(args[1], args[1]);
	if (error_status != 0)
	{
		// `.` komutu, execve hataları yerine genel hata kodu (1) kullanır.
		return (1);
	}
	
	// ... Dosyayı "source" etme mantığı burada olurdu ...
	// Şimdilik, hata yoksa başarılı kabul ediyoruz.
	return (0);
}