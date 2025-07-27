/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:54:10 by nuciftci          #+#    #+#             */
/*   Updated: 2025/07/27 16:00:59 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"


//  ft_pwd - Mevcut çalışma dizinini standart çıktıya yazdırır.
 
//  Bu fonksiyon, POSIX standardında yer alan getcwd() fonksiyonunu kullanarak
//  mevcut çalışma dizininin tam yolunu alır. Alınan bu yolu ekrana yazdırır
//  ve sonuna bir yeni satır karakteri ekler.
 
//  getcwd(NULL, 0) kullanımı, fonksiyonun yol için gereken belleği kendisinin
//  (malloc ile) ayırmasını sağlar. Bu, bizim buffer boyutunu tahmin etme
//  zorunluluğumuzu ortadan kaldırır. Ancak bu durumda, işimiz bittiğinde
//  bu belleği free() ile serbest bırakmamız gerekir.

//  Döner Değer:
//    - Başarılı olursa 0 döndürür.
//    - getcwd() bir hata nedeniyle başarısız olursa (örneğin, üst dizinlerden
//      birinin okuma izni yoksa), hata mesajı yazdırır ve 1 döndürür.
 


int ft_pwd(void)
{
	char	*current_dir;
	
	// getcwd'ye NULL ve 0 parametrelerini vererek, yol için gereken belleği
	// otomatik olarak ayırmasını sağlıyoruz.
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		// getcwd başarısız olursa, NULL döner ve errno'yu ayarlar.
		// perror, "minishell: pwd" metninin yanına sistem hata mesajını
		// (örn: "No such file or directory") otomatik olarak ekler.
		perror("minishell: pwd");
		return (1); // Hata durumunu çağıran fonksiyona bildir.
	}
	
	// Başarıyla alınan yolu ekrana yazdır.
	printf("%s\n", current_dir);

	// getcwd tarafından malloc ile ayrılan belleği serbest bırak.
	// Bu adım, bellek sızıntılarını (memory leaks) önlemek için kritiktir
	free(current_dir);
	
	return (0);
}
