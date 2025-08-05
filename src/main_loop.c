/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:42:35 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/05 17:58:16 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// src/main_loop.c

#include "minishell.h"

/*
 * ANA İŞLEVSELLİK DÖNGÜSÜ
 * Görevleri:
 * 1. Kullanıcıdan komut satırını okumak.
 * 2. Lexer, Parser, Expander, Executor adımlarını sırayla çalıştırmak.
 * 3. Her komuttan sonra ilgili bellek alanlarını temizlemek.
 */
void	main_loop(t_shell *shell)
{
	char		*line;
	t_token		*tokens;
	t_command_chain	*cmd_tree;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line) // Ctrl+D basıldı (EOF)
		{
			printf("exit\n");
			break; // Döngüyü kır ve programı sonlandır.
		}
		if (*line)
			add_history(line);
		else // Boş satır girildiyse, belleği temizle ve döngüye baştan başla.
		{
			free(line);
			continue;
		}
		tokens = lexer(line);
		// print_tokens(tokens); 
		if (!tokens) // Sözdizimi hatası veya boş satır
		{
			free(line);
			continue;
		}
		cmd_tree = parser(tokens);
		free_token_list(tokens); // Token listesine artık ihtiyacımız yok.
		
		if (!cmd_tree) // Parser hatası
		{
			free(line);
			// `shell->exit_code = 258` (syntax error) gibi bir atama yapılabilir.
			continue;
		}
		free(line);

		// Expander ve Executor, shell durumuna (env, exit_code) ihtiyaç duyar.
		expander(cmd_tree, shell);
		executor(cmd_tree, shell);
		if (shell->exit_code == SHELL_SHOULD_EXIT)
		{
    		free_cmd_tree(cmd_tree);
    		break; // Döngüyü kır, ana temizlik main()'de yapılacak.
		}

		// // Bu komut için ayrılan her şeyi temizle.
		free_cmd_tree(cmd_tree);
	}
}