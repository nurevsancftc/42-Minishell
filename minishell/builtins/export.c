/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:19:35 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/01 04:28:46 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdio.h>

/*
 * NOT: Bu dosyanın doğru çalışması için, projenizin başka bir yerinde
 * (örn: env_utils.c) tanımlanmış olan ve ortam değişkenlerini yöneten
 * `find_env_node` ve `update_or_create_env` gibi yardımcı fonksiyonlara
 * ihtiyaç duyulur. Bu fonksiyonların prototipleri `minishell.h` veya
 * `builtins.h` gibi genel bir başlık dosyası aracılığıyla bu dosyaya
 * dahil edilir.
 */

/* ========================================================================== */
/*           SADECE BU DOSYADA KULLANILAN YARDIMCI FONKSİYONLAR (STATIC)      */
/* ========================================================================== */
/* `static` anahtar kelimesi, bu fonksiyonların sadece bu .c dosyası içinden
 * çağrılabileceği anlamına gelir. Bu, kodumuzu daha modüler ve güvenli yapar. */

/**
 * print_sorted_env - `export` komutu argümansız çağrıldığında çalışır.
 *
 * Görevi, tüm ortam değişkenlerini `declare -x ANAHTAR="DEĞER"` formatında
 * ekrana yazdırmaktır. Bu, kabuğun durumunu dışarıya aktarmak için kullanılır.
 *
 * NOT: Bu basit versiyon, değişkenleri alfabetik olarak SIRALAMAZ.
 * Sıralama, projenin bonus kısmı veya daha ileri bir adımı olarak kabul
 * edilebilir. Zorunlu kısım için genellikle bu kadarı yeterlidir.
 */
static void	print_sorted_env(t_shell *shell)
{
	t_list	*node;
	t_env	*env;

	node = shell->env_list;
	while (node)
	{
		env = (t_env *)node->content;
		// Değeri olan değişkenler "değer" formatında yazdırılır.
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		// "export VAR" gibi komutlarla oluşturulmuş, değeri olmayanlar ise
		// sadece anahtar olarak yazdırılır.
		else
			printf("declare -x %s\n", env->key);
		node = node->next;
	}
}

/**
 * is_valid_identifier - Bir anahtarın geçerli bir kabuk değişken adı olup
 *                       olmadığını kontrol eder.
 *
 * Bir değişken adının geçerli olması için kurallar şunlardır:
 * 1. İlk karakter bir harf (a-z, A-Z) veya alt çizgi (_) olmalıdır.
 * 2. Sonraki karakterler harf, rakam (0-9) veya alt çizgi olabilir.
 *
 * Örnekler:
 * - GEÇERLİ: VAR, _VAR, VAR123
 * - GEÇERSİZ: 123VAR, VAR-NAME, ?VAR
 */
static int	is_valid_identifier(const char *key)
{
	int i;

	i = 0;
	// Anahtar boş mu veya ilk karakteri geçersiz mi?
	if (!key || !(ft_isalpha(key[i]) || key[i] == '_'))
		return (0);
	i++;
	// Geriye kalan karakterleri kontrol et.
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0); // Geçersiz bir karakter bulundu.
		i++;
	}
	return (1); // Tüm kontrollerden geçti, isim geçerli.
}

/**
 * handle_export_argument - `export`'a verilen tek bir argümanı işler.
 *
 * Örnek argümanlar: "USER=test" veya "MY_VAR"
 *
 * Adımlar:
 * 1. Argümanda '=' işareti olup olmadığını arar.
 * 2. '=' varsa, string'i o noktadan "key" ve "value" olarak ikiye ayırır.
 * 3. '=' yoksa, tüm argümanı "key" olarak alır, "value" NULL olur.
 * 4. Anahtarın geçerli bir isim olup olmadığını `is_valid_identifier` ile kontrol eder.
 * 5. Her şey yolundaysa, `update_or_create_env`'i çağırarak işlemi tamamlar.
 */
static int	handle_export_argument(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	char	*equals_sign;

	equals_sign = ft_strchr(arg, '=');
	if (equals_sign != NULL)
	{
		// "KEY=VALUE" durumu
		*equals_sign = '\0'; // '=' yerine NULL koyarak string'i böl.
		key = arg;
		value = equals_sign + 1;
	}
	else
	{
		// Sadece "KEY" durumu
		key = arg;
		value = NULL;
	}
	if (is_valid_identifier(key) == 0)
	{
		fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", key);
		return (1); // Hata durumu
	}
	// "export VAR" komutunda, eğer VAR zaten varsa bir şey yapma. Bu bash davranışıdır.
	if (value == NULL && find_env_node(shell->env_list, key) != NULL)
		return (0);

	// Değişkeni güncelle veya oluştur.
	update_or_create_env(shell, key, value);
	return (0); // Başarılı
}


/* ========================================================================== */
/*                DIŞARIDAN ÇAĞIRILACAK ANA ft_export FONKSİYONU              */
/* ========================================================================== */
/* Bu fonksiyonun prototipi `builtins.h` içinde yer alır ve projenin diğer
 * kısımlarından (özellikle executor'dan) çağrılır. */

/**
 * ft_export - 'export' yerleşik komutunun ana giriş noktası.
 *
 * Bu fonksiyon bir yönetici gibi çalışır:
 * - Argüman yoksa listeleme modunu çalıştırır.
 * - Argüman varsa, her birini tek tek işlemesi için yardımcı fonksiyona yollar.
 */
int	ft_export(char **args, t_shell *shell)
{
	int	i;
	int	final_status; // `export a=1 bbb=2 ccc` gibi komutlarda bir tanesi bile
	                  // hatalıysa, fonksiyonun en sonunda 1 dönmesi gerekir.

	final_status = 0; // Başlangıçta her şeyin yolunda olduğunu varsay.
	i = 1; // Argümanları işlemeye `args[1]`'den başla.

	// Argüman yoksa, `export` tek başına çağrılmıştır.
	if (args[1] == NULL)
	{
		print_sorted_env(shell);
		return (0);
	}

	// Argüman varsa, her birini döngü ile işle.
	while (args[i] != NULL)
	{
		// Her bir argümanı işlemesi için yardımcıyı çağır.
		if (handle_export_argument(args[i], shell) != 0)
			final_status = 1; // Herhangi bir hata olursa durumu kaydet.
		i++;
	}

	return (final_status); // Son durumu döndür.
}