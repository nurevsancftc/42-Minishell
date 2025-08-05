/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:49:43 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/05 13:32:42 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h> // access() ve X_OK için

#include <stdlib.h>

void	ft_free_array(char **split_array)
{
	int	i;

	if (!split_array)
		return;
	i = 0;
	while (split_array[i])
	{
		free(split_array[i]);
		i++;
	}
	free(split_array);
}

/**
 * try_each_path - Verilen yollar dizisinde komutu arar.
 *
 * Bu fonksiyon, `PATH`'ten gelen her bir dizini komutla birleştirir
 * ve çalıştırılabilir bir dosya olup olmadığını kontrol eder.
 * İlk bulduğunu döndürür.
 *
 * `static`'tir çünkü sadece `get_command_path` tarafından kullanılır.
 */

static char	*try_each_path(char *cmd, char **paths)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = -1;
	while (paths && paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (full_path); // Çalıştırılabilir yol bulundu.
		free(full_path);
	}
	return (NULL); // Hiçbir yolda bulunamadı.
}

/**
 * get_command_path - Bir komut adını, çalıştırılabilir tam yola dönüştürür.
 *
 * Bu fonksiyon 25 satır kuralına uymak için asıl arama işini
 * `try_each_path` yardımcısına devreder.
 */
char	*get_command_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*path_var;
	char	*executable_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_var = get_env_value(shell->env_list, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	executable_path = try_each_path(cmd, paths);
	if (paths)
		ft_free_array(paths);
	return (executable_path);
}

/**
 * populate_env_array - Hazırlanmış `env_array`'i `t_env` listesinden doldurur.
 *
 * `static`'tir çünkü sadece `convert_env_list_to_array` tarafından kullanılır.
 */
static int	populate_env_array(char **env_array, t_list *env_list)
{
	t_env	*env;
	char	*temp;
	int		i;

	i = 0;
	while (env_list)
	{
		env = (t_env *)env_list->content;
		if (env->value) // Sadece değeri olanları işle
		{
			temp = ft_strjoin(env->key, "=");
			if (!temp)
				return (-1); // Malloc hatası
			env_array[i] = ft_strjoin(temp, env->value);
			free(temp); // ft_strjoin'den gelen geçici string'i unutma
			if (!env_array[i])
				return (-1); // Malloc hatası
			i++;
		}
		env_list = env_list->next;
	}
	env_array[i] = NULL; // Diziyi NULL ile sonlandır.
	return (0);
}

/**
 * count_valid_env_vars - Değeri NULL olmayan ortam değişkenlerinin sayısını bulur.
 */
static int	count_valid_env_vars(t_list *env_list)
{
	int		count;
	t_env	*env;

	count = 0;
	while (env_list)
	{
		env = (t_env *)env_list->content;
		// Sadece değeri olanları say
		if (env->value)
			count++;
		env_list = env_list->next;
	}
	return (count);
}

/**
 * convert_env_list_to_array - t_env listesini `execve`'nin beklediği
 *                             `char **` formatına dönüştürür.
 *
 * 25 satır kuralına uymak için bellek ayırma ve doldurma işlemlerini ayırır.
 */
char	**convert_env_list_to_array(t_shell *shell)
{
	char	**env_array;
	int		valid_vars_count;

	// 1. ADIM: Sadece geçerli olanları say
	valid_vars_count = count_valid_env_vars(shell->env_list);

	// 2. ADIM: Doğru boyutta bellek ayır
	env_array = malloc(sizeof(char *) * (valid_vars_count + 1));
	if (!env_array)
		return (NULL);

	// 3. ADIM: Diziyi doldur ve hata kontrolü yap
	if (populate_env_array(env_array, shell->env_list) == -1)
	{
		// Doldurma sırasında bir malloc hatası oldu.
		// O ana kadar doldurulanları ve dizinin kendisini temizle.
		ft_free_array(env_array);
		return (NULL);
	}

	return (env_array);
}
