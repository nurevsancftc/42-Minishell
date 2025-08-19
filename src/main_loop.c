/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:42:35 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 18:22:12 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// src/main_loop.c

#include "minishell.h"

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
		setup_interactive_signals();
		line = readline("minishell$ ");

		if (g_status == STATUS_CTRL_C)
		{
			shell->exit_code = 130;   // `$?`'ı 1 yap.
			g_status = STATUS_OK;   // Durumu sıfırla.
		}
		if (!line) // Ctrl+D basıldı.
		{
			ft_putstr_fd("exit\n", 2);
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
