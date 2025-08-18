/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:54:10 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 18:55:00 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/**
 * ft_pwd - Mevcut çalışma dizinini ekrana basar.
 *
 * Bu versiyon, komutun herhangi bir argüman alıp almadığını kontrol eder.
 * Eğer 'pwd' komutundan sonra bir argüman verilirse (örn: "pwd foo"),
 * bash gibi bir hata mesajı basar ve işlem yapmaz.
 *
 * Parametreler:
 *   args - Komut ve argümanlarını içeren string dizisi.
 *          Parser'dan bu şekilde gelecek (args[0] = "pwd").
 *
 * Döner Değer:
 *   - Başarılı olursa 0.
 *   - Hata olursa (argüman verilmesi veya getcwd hatası) 1.
 */
int		ft_pwd(char **args, t_shell *shell)
{
	char	*current_dir;
	(void)shell;
	(void)args;

	// 1. Argüman kontrolü yapalım.
	// `args` dizisinin ikinci elemanı (args[1]) varsa, bu,
	// komutun "pwd deneme" gibi bir argümanla çağrıldığı anlamına gelir.


	// 2. Mevcut dizini al. Bu kısım senin kodunda zaten doğruydu.
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		perror("minishell: pwd");
		return (1);
	}

	// 3. Ekrana yazdır ve belleği serbest bırak.
	printf("%s\n", current_dir);
	free(current_dir);

	return (0); // İşlem başarılı.
}
