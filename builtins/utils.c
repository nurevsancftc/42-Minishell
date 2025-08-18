/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:15:29 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 19:02:48 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>


/* ========================================================================== */
/*                   CD İÇİN GEREKLİ YARDIMCI FONKSİYONLAR                    */
/* ========================================================================== */
/*
 * NOT: Bu yardımcı fonksiyonlar normalde projenin genelinde kullanılmak
 * üzere `env_utils.c` gibi ayrı bir dosyada bulunur. Ancak `cd`'nin
 * mantığını tek bir yerde görebilmek için buraya dahil edilmiştir.
 */

/**
 * find_env_node - Ortam değişkenleri listesinde belirli bir anahtarı arar.
 *
 * Bu fonksiyon, `t_list` üzerinde döner ve her düğümün `content`'ini
 * bir `t_env` yapısına cast ederek anahtar karşılaştırması yapar.
 *
 * Parametreler:
 *   env_list: Ortam değişkenlerini tutan listenin başlangıcı.
 *   key: Aranacak anahtar (örn: "HOME", "PWD").
 *
 * Döner Değer:
 *   Anahtarı içeren `t_list` düğümünü bulursa onun adresini, bulamazsa NULL döner.
 */
t_list	*find_env_node(t_list *env_list, const char *key)
{
	t_env	*current_env;

	while (env_list)
	{
		current_env = (t_env *)env_list->content;
		if (current_env && current_env->key && strcmp(current_env->key, key) == 0)
		{
			return (env_list); // Aranan düğüm bulundu.
		}
		env_list = env_list->next;
	}
	return (NULL); // Bulunamadı.
}

/**
 * get_env_value - Bir ortam değişkeninin değerini string olarak döndürür.
 *
 * Önce `find_env_node` ile ilgili düğümü bulur, sonra o düğümün
 * içindeki `value` alanını döndürür.
 */
char	*get_env_value(t_list *env_list, const char *key)
{
	t_list	*node;
	t_env	*env_var;

	node = find_env_node(env_list, key);
	if (node == NULL)
	{
		return (NULL); // Değişken listede yok.
	}
	
	env_var = (t_env *)node->content;

	// EKSTRA KONTROL: Değişken var ama değeri NULL mı? (örn: export VAR)
	if (env_var->value == NULL)
	{
		return (NULL); // veya return ("") de bir seçenek olabilir.
	}

	return (env_var->value);
}



/**
 * is_valid_identifier - Bir anahtarın geçerli bir kabuk adı olup olmadığını
 *                       kontrol eder. (export ve unset kullanabilir)
 *                       Not: Bu fonksiyonu `export` için yazdığımızdan
 *                       farklı olarak, '=' kontrolü yapmaz.
 *                       Daha genel bir isim verilebilir.
 */
int	is_valid_identifier(const char *key)
{
	int i;

	i = 0;
	if (!key || !(ft_isalpha(key[i]) || key[i] == '_'))
		return (0);
	i++;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/**
 * handle_dollar_sign - Bir '$' karakteriyle karşılaşıldığında çağrılır.
 * Değişkeni okur, değerini bulur ve döndürür.
 */
/**
 * handle_dollar_sign - Bir '$' ile karşılaşıldığında değişkeni işler.
 *
 * '$' sonrası karakterlere bakarak değişken adını (key) çıkarır,
 * bu key'e karşılık gelen değeri bulur ve bu değeri yeni bir string
 * olarak döndürür. İmleci (line_ptr) değişken adının sonuna ilerletir.
 *
 * @param line_ptr: İşlenen satırın geri kalanını gösteren işaretçiye işaretçi.
 * @param shell: Ortam değişkenlerine ve son çıkış koduna erişim için.
 * @return: Genişletilmiş değişkenin değerini (malloc ile) döndürür.
 *          Bulunamazsa veya geçersizse boş bir string döndürür.
 */
char	*handle_dollar_sign(const char **line_ptr, t_shell *shell)
{
	const char	*start;
	char		*key;
	char		*value;

	(*line_ptr)++; // '$' karakterini atla.

	// Özel durum: $?
	if (**line_ptr == '?')
	{
		(*line_ptr)++; // '?' karakterini atla.
		return (ft_itoa(shell->exit_code));
	}
	
	// Özel durum: Sadece '$' veya '$' sonrası geçersiz karakter
	// (örn: "$ ", "$/"). Değişken adı harf, rakam veya '_' ile başlamalı.
	if (!ft_isalpha(**line_ptr) && **line_ptr != '_')
		return (ft_strdup("$"));

	// Değişken adının başlangıcını işaretle.
	start = *line_ptr;
	// Değişken adı harf, rakam veya alt çizgi içerir.
	while (ft_isalnum(**line_ptr) || **line_ptr == '_')
		(*line_ptr)++;

	// Başlangıç ve son arasındaki kısmı değişken adı (key) olarak al.
	key = ft_substr(start, 0, *line_ptr - start);
	if (!key)
		return (ft_strdup("")); // Malloc hatası

	// Ortam değişkenleri listesinden bu key'in değerini bul.
	value = get_env_value(shell->env_list, key);
	free(key);

	if (!value) // Değişken bulunamadıysa
		return (ft_strdup("")); // Boş bir string döndür.
	
	// Değerin bir kopyasını döndür.
	return (ft_strdup(value));
}

/**
 * expand_heredoc_line - Heredoc için bir satırdaki değişkenleri genişletir.
 *
 * Bu fonksiyon, bir satırı karakter karakter gezer.
 * '$' ile başlayan değişkenleri bulur, değerleriyle değiştirir ve
 * sonuçları birleştirerek yeni bir string oluşturur.
 *
 * @param line: Kullanıcı tarafından girilen orijinal satır.
 * @param shell: Ortam değişkenlerine erişim için ana yapı.
 * @return: Genişletilmiş yeni satırı (malloc ile) döndürür.
 */
char	*expand_heredoc_line(char *line, t_shell *shell)
{
	char	*final_str;
	char	*temp_str;
	char	*var_value;
	int		i;
	int		j;

	final_str = ft_strdup(""); // Sonuç string'ini boş olarak başlat.
	i = 0;
	while (line[i])
	{
		// Bir sonraki '$'e kadar olan normal kısmı bul.
		j = i;
		while (line[j] && line[j] != '$')
			j++;
		
		// Normal kısmı (varsa) al ve sonuç string'ine ekle.
		if (j > i)
		{
			temp_str = ft_substr(line, i, j - i);
			char *new_final = ft_strjoin(final_str, temp_str);
			free(final_str);
			free(temp_str);
			final_str = new_final;
		}

		// Eğer bir '$' bulduysak, onu işle.
		if (line[j] == '$')
		{
			const char *line_ptr = &line[j];
			var_value = handle_dollar_sign(&line_ptr, shell);
			char *new_final = ft_strjoin(final_str, var_value);
			free(final_str);
			free(var_value);
			final_str = new_final;
			// İmleci, değişkenin işlendiği yere ilerlet.
			i = line_ptr - line;
		}
		else // Satırın sonuna geldik.
		{
			i = j;
		}
	}
	return (final_str);
}


/**
 * update_or_create_env - Bir ortam değişkenini günceller veya yoksa yenisini oluşturur.
 *
 * Bu fonksiyon, `cd` ve `export` gibi komutların temelini oluşturur.
 */
void	update_or_create_env(t_shell *shell, const char *key, const char *value)
{
	t_list	*node;
	t_env	*env;

	node = find_env_node(shell->env_list, key);
	if (node != NULL) // Değişken zaten varsa
	{
		env = (t_env *)node->content;
		if (value != NULL) // Sadece yeni bir değer varsa güncelle
		{
			free(env->value);
			env->value = ft_strdup(value);
		}
	}
	else // Değişken yoksa, yenisini oluştur
	{
		env = ft_calloc(1, sizeof(t_env));
		if (!env)
			return;
		env->key = ft_strdup(key);
		if (value != NULL)
			env->value = ft_strdup(value);
		// Değer NULL ise, env->value zaten calloc ile NULL'a ayarlandı.
		node = ft_lstnew(env);
		ft_lstadd_back(&(shell->env_list), node);
	}
}


/**
 * ft_check_path_error - Bir yolun durumunu kontrol eder ve gerekirse hata basar.
 *
 * Parametreler:
 *   path - Kontrol edilecek yol.
 *   cmd_name - Hata mesajında kullanılacak komut adı (örn: "ls", ".").
 *
 * Döner Değer:
 *   - 0: Yol geçerli bir dosyaya benziyor, sorun yok.
 *   - 126: Yol bir dizindir.
 *   - 127: Yol mevcut değildir.
 */
int	ft_check_path_error(const char *path, const char *cmd_name)
{
	struct stat	path_stat;

	// 1. Dosyanın/dizinin var olup olmadığını kontrol et.
	// stat, hem varlığı hem de türünü tek seferde kontrol etmemizi sağlar.
	if (stat(path, &path_stat) != 0)
	{
		// stat başarısız olduysa, bunun nedenini perror ile öğrenmek en iyisidir.
		// Genellikle "No such file or directory" olur.
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)cmd_name, 2);
		ft_putstr_fd(": ", 2);
		perror(NULL); // Örn: "No such file or directory"
		return (127);
	}

	// 2. Eğer varsa, bunun bir dizin olup olmadığını kontrol et.
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)cmd_name, 2);
		ft_putstr_fd(": is a directory\n", 2);
		return (126);
	}

	// 3. Dosya var, dizin değil, peki çalıştırma izni var mı?
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)cmd_name, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (126);
	}

	// Her şey yolunda, hata yok.
	return (0);
}