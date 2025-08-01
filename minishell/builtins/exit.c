/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:45:07 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/01 02:34:07 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>      // printf
#include <stdlib.h>     // exit, atoi
#include <string.h>     // strlen
#include <unistd.h>     // write

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
 * Kabuğu sonlandırır ve bir çıkış kodu belirler.
 * Argümanların geçerliliğini kontrol eder ve bash'in davranışını taklit eder.
 */
int	ft_exit(char **args)
{
	int	exit_status;

	// Gerçek bir kabuk gibi, önce "exit" yazısını ekrana basalım.
	printf("exit\n");

	// 1. DURUM: "exit" tek başına mı kullanıldı?
	if (args[1] == NULL)
	{
		// Argüman yoksa, başarı kodu olan 0 ile çık.
		exit(0);
	}

	// 2. DURUM: Verilen argüman sayısal değil mi? (örn: "exit hello")
	if (is_numeric(args[1]) == 0)
	{
		// Hata mesajını standart hata çıktısına (stderr) yaz.
		write(2, "minishell: exit: ", 17);
		write(2, args[1], strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		// Bash bu durumda 255 koduyla çıkar.
		exit(255);
	}

	// 3. DURUM: Çok fazla argüman mı var? (örn: "exit 50 world")
	if (args[2] != NULL)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		// Bu durumda kabuktan ÇIKMIYORUZ, sadece hata verip devam ediyoruz.
		// Hata olduğunu belirtmek için 1 döndürürüz.
		return (1);
	}

	// 4. DURUM: Her şey yolunda, tek bir sayısal argüman var.
	// "atoi" fonksiyonu string'i integer'a çevirir.
	exit_status = atoi(args[1]);

	// Çıkış kodları 0 ile 255 arasında olmalıdır.
	// (unsigned char) cast'i, sayıyı otomatik olarak bu aralığa sığdırır.
	// Örneğin 257 girilirse, 1 olarak çıkar. Bu, doğru davranıştır.
	exit((unsigned char)exit_status);
}