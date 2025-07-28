/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:42:35 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/07/28 18:51:33 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// src/main_loop.c

#include "minishell.h"

// Her döngünün sonunda komut için ayrılan belleği temizleyen yardımcı.
static void	cleanup_command_data(t_cmd_table *cmd_tree, t_token *tokens, char *line)
{
	if (cmd_tree)
		free_cmd_tree(cmd_tree); // AST'yi temizle
	// NOT: parser'da hata olursa token listesi oluşmuş ama cmd_tree oluşmamış olabilir.
	// if (tokens)
	//     free_token_list(tokens);
	if (line)
		free(line);
}


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
	t_cmd_table	*cmd_tree;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line) // Ctrl+D basıldı (EOF)
		{
			printf("exit\n");
			break;
		}
		if (*line)
			add_history(line);
		
		tokens = lexer(line);
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

		// Expander ve Executor, shell durumuna (env, exit_code) ihtiyaç duyar.
		expander(cmd_tree, shell);
		executor(cmd_tree, shell);

		// Bu komut için ayrılan her şeyi temizle.
		free_cmd_tree(cmd_tree);
		free(line);
	}
}