/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:00:19 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/01 02:21:04 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "builtins.h"
#include <stdio.h>

/**
 * is_it_an_n_option - Bir argümanın "-n" veya "-nnnn" gibi bir şey olup
 *                     olmadığını kontrol eden basit bir yardımcı.
 *
 * Sadece '-' ile başlayıp sadece 'n' harfleriyle devam ediyorsa 1 döner.
 * Yoksa 0 döner.
 */
static int	is_it_an_n_option(char *arg)
{
	int	i;

	// Eğer argüman '-' ile başlamıyorsa, direkt geç.
	if (arg[0] != '-')
		return (0);

	// İkinci karakterden itibaren kontrol et.
	i = 1;
	while (arg[i] != '\0')
	{
		// Eğer 'n' dışında bir harf bulursak, bu bir -n seçeneği değildir.
		if (arg[i] != 'n')
			return (0);
		i++;
	}

	// Eğer döngü bittiyse ve hiç 'n' dışında harf bulamadıysak,
	// ve string boş değilse (sadece '-' gibi), o zaman bu bir -n seçeneğidir.
	// i > 1 kontrolü, sadece "-" girilmesini engeller.
	if (i > 1)
		return (1);
	else
		return (0);
}

/**
 * ft_echo - 'echo' komutunun daha basit, adımlara bölünmüş bir versiyonu.
 *
 * Bu kod, en optimize şekilde değil, mantığın daha kolay takip edilebileceği
 * şekilde yazılmıştır.
 */
int	ft_echo(char **args)
{
	int	i;
	int	found_n_option; // bool yerine int flag kullanmak daha yaygındır.
	int	first_word_printed;

	i = 1; // 0. argüman "echo" olduğu için 1'den başla.
	found_n_option = 0; // Başta -n seçeneği yokmuş gibi davran.
	first_word_printed = 0; // Henüz ekrana bir kelime basmadık.

	// 1. ADIM: Önce -n seçeneklerini atla.
	// Bu döngü sadece komutun en başındaki -n'leri bulur ve geçer.
	while (args[i] != NULL)
	{
		if (is_it_an_n_option(args[i]))
		{
			found_n_option = 1; // Bir tane bulduk, artık sona \n koymayacağız.
			i++;
		}
		else
		{
			// -n olmayan ilk kelimeyi bulduğumuz an bu döngüden çık.
			break;
		}
	}

	// 2. ADIM: Şimdi kelimeleri yazdırmaya başla.
	// Bu döngü, -n'lerin bittiği yerden devam eder.
	while (args[i] != NULL)
	{
		// Eğer bu ekrana basacağımız ilk kelime değilse,
		// kelimelerin arasına boşluk koymak için ÖNCE boşluğu bas.
		if (first_word_printed == 1)
		{
			printf(" ");
		}
		
		// Kelimeyi ekrana bas.
		printf("%s", args[i]);
		
		// Ekrana ilk kelimeyi bastığımızı işaretle.
		first_word_printed = 1;

		i++;
	}

	// 3. ADIM: Sona yeni satır eklenip eklenmeyeceğine karar ver.
	// Eğer döngünün en başında hiç -n seçeneği bulamadıysak
	// (yani found_n_option hala 0 ise), o zaman sona yeni satır ekle.
	if (found_n_option == 0)
	{
		printf("\n");
	}

	return (0);
}
