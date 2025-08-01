/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:13:00 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/01 05:13:05 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * remove_node_from_list - Verilen anahtarı listede bulur ve siler.
 *
 * Bu fonksiyon, klasik "önceki düğümü takip et" yöntemiyle çalışır.
 * Düğüm bulunduğunda, bağlantıyı koparır ve düğümü bellekten siler.
 *
 * Parametreler:
 *   key_to_remove: Silinecek ortam değişkeninin anahtarı.
 *   shell: Ana kabuk yapısı (env_list'e erişim için).
 */
static void	remove_node_from_list(const char *key_to_remove, t_shell *shell)
{
	t_list	*current;
	t_list	*previous;

	previous = NULL;
	current = shell->env_list;
	while (current != NULL)
	{
		if (strcmp(((t_env *)current->content)->key, key_to_remove) == 0)
		{
			// Aranan düğümü bulduk, şimdi silelim.
			if (previous == NULL) // Listenin başını siliyoruz.
				shell->env_list = current->next;
			else // Aradan bir düğümü siliyoruz.
				previous->next = current->next;
			// ft_lstdelone, hem düğümü hem de içeriğini (free_env_content ile) siler.
			ft_lstdelone(current, free_env_content);
			return; // İşimizi bitirdik, fonksiyondan çık.
		}
		previous = current;
		current = current->next;
	}
	// Eğer döngü bittiyse ve düğüm bulunamadıysa, hiçbir şey yapmayız.
}

/**
 * ft_unset - 'unset' yerleşik komutunu çalıştırır.
 *
 * Bu fonksiyon, argümanları kontrol eden ve her birini silmesi için
 * `remove_node_from_list`'i çağıran bir yöneticidir.
 * 25 satır kuralına uyar.
 */
int	ft_unset(char **args, t_shell *shell)
{
	int		i;
	int		exit_status;

	i = 1;
	exit_status = 0;
	// Argüman listesini gez.
	while (args[i] != NULL)
	{
		// Her argümanın geçerli bir isim olup olmadığını kontrol et.
		if (is_valid_identifier(args[i]) == 0)
		{
			fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n", \
				args[i]);
			exit_status = 1; // Hata durumunu kaydet.
		}
		else
		{
			// İsim geçerliyse, silme işlemini yapması için yardımcıyı çağır.
			remove_node_from_list(args[i], shell);
		}
		i++;
	}
	return (exit_status);
}
