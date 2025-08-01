/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:26:50 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/01 08:54:27 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * executor - Komut zincirini alır ve her bir komutu çalıştırır.
 *
 * Bu fonksiyon bir orkestra şefi gibidir. Parser'dan gelen komut
 * ağacını alır ve her bir komut için karar verir: "Bu bir built-in mi?
 * Yoksa harici bir program mı?". Ardından ilgili yürütücüyü çağırır.
 *
 * NOT: Bu basit versiyon, şimdilik sadece tek bir komutu (pipe'sız)
 * ele almaktadır. Pipe'lar eklendiğinde bu fonksiyonun genişletilmesi
 * gerekecektir.
 */
void	executor(t_command_chain *cmd_chain, t_shell *shell)
{
	char	**args;

	// Güvenlik kontrolü: Geçersiz bir komut ağacı gelirse hiçbir şey yapma.
	if (cmd_chain == NULL || cmd_chain->simple_command == NULL)
		return;

	// Çalıştırılacak komutun argümanlarını al.
	args = cmd_chain->simple_command->args;

	// Güvenlik kontrolü: Argüman listesi boşsa hiçbir şey yapma.
	if (args == NULL || args[0] == NULL)
		return;

	// Karar anı: Bu bir yerleşik komut mu?
	if (execute_builtin(args, shell) == 1)
	{
		// Evet, `execute_builtin` komutu bulup çalıştırdı.
		// İşimiz burada bitti.
		return;
	}
	
	// Hayır, bu bir yerleşik komut değil.
	// O zaman harici komut yürütücüsünü çağır.
	execute_external(args, shell);
}