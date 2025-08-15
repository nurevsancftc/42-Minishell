/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:42:35 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/15 18:29:05 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// src/main_loop.c

#include "minishell.h"

/**
 * @brief Bir komut satırını baştan sona işleyen ana yardımcı fonksiyon.
 * 
 * Bu fonksiyon, `main_loop`'u temiz tutmak için tüm işlem hattını
 * (lexer, parser, expander, executor) yönetir ve kendi kaynaklarını
 * temizler.
 *
 * @param line Kullanıcı tarafından girilen ham komut satırı.
 * @param shell Kabuğun genel durumu.
 */
static int	process_line(char *line, t_shell *shell)
{
	t_token	*tokens;
	int		status;

	tokens = NULL;
	status = 0;
	shell->cmd_tree = NULL;
	if (*line)
	{
		add_history(line);
		tokens = lexer(line);
		if (tokens)
		{
			shell->cmd_tree = parser(tokens);
			free_token_list(tokens);
			if (shell->cmd_tree)
			{
				// --- DOĞRU SIRA BURASI ---
				// 1. Parser'dan sonra, komutları genişlet.
				expander(shell->cmd_tree, shell);

				// 2. Genişletilmiş komutları çalıştır.
				status = executor(shell->cmd_tree, shell);
			}
			else
				shell->exit_code = 2; // Syntax hatası
		}
		else
			shell->exit_code = 2; // Lexer hatası
	}
	free_cmd_tree(shell->cmd_tree);
	shell->cmd_tree = NULL;
	return (status);
}

/**
 * @brief Kabuğun ana komut okuma ve yürütme döngüsü.
 *        Bu versiyon, döngü mantığını ve kaynak yönetimini ayırır.
 */
void	main_loop(t_shell *shell)
{
	char	*line;
	int		status;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line) // Ctrl+D basıldı.
		{
			printf("exit\n");
			break; // Döngüyü sonlandır.
		}

		// Asıl işi yapan yardımcı fonksiyonu çağır.
		status = process_line(line, shell);

		// `line` readline tarafından malloc ile ayrıldığı için her döngüde
		// serbest bırakılmalıdır.
		free(line);

		// Eğer "exit" komutu çağrıldıysa, döngüyü sonlandır.
		if (status == SHELL_SHOULD_EXIT)
			break;
	}
}
