/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:48:26 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/03 10:22:11 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * child_routine - Her bir çocuk sürecin çalıştıracağı ana mantık.
 *
 * Bu fonksiyon, G/Ç yönlendirmelerini (ileride eklenecek) ve
 * komutun kendisini (built-in veya harici) çalıştırır.
 * Başarılı olursa `exit` ile sonlanır, asla geri dönmez.
 */
static void	child_routine(t_simple_command *cmd, t_shell *shell)
{
	int	exit_code;

	// Ileride buraya I/O Redirection (>, <, >>) kodları gelecek.
	// handle_redirections(cmd);
	if (execute_builtin(cmd->args, shell))
	{
		exit_code = shell->exit_code;
		// Bellek temizliği (eğer gerekiyorsa)
		exit(exit_code);
	}
	execute_external_in_child(cmd->args, shell); // Bu execve yapar ve çıkmazsa hata verir.
}

/**
 * wait_for_children - Oluşturulan tüm çocuk süreçlerin bitmesini bekler
 *                     ve son komutun çıkış kodunu alır.
 */
static void	wait_for_children(int last_pid, t_shell *shell)
{
	int	status;

	// Önce son komutu bekle, onun çıkış kodu $? olacak.
	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	// Geriye kalan diğer tüm çocuk süreçlerin bitmesini bekle (zombi olmasınlar).
	while (wait(NULL) > 0)
		;
}

/**
 * execute_pipeline - Komut zincirini `pipe`'lar aracılığıyla çalıştırır.
 *
 * Her komut için bir `fork` oluşturur ve `pipe`/`dup2` ile G/Ç'yi bağlar.
 */
void	execute_pipeline(t_command_chain *cmd_chain, t_shell *shell)
{
	int		pipe_fds[2];
	int		input_fd;
	pid_t	last_pid;

	input_fd = STDIN_FILENO;
	while (cmd_chain)
	{
		if (cmd_chain->next) // Son komut değilse pipe oluştur.
			pipe(pipe_fds);
		last_pid = fork();
		if (last_pid == -1)
			return (perror("minishell: fork"));
		if (last_pid == 0) // Çocuk Süreç
		{
			if (input_fd != STDIN_FILENO)
				dup2(input_fd, STDIN_FILENO);
			if (cmd_chain->next)
				dup2(pipe_fds[1], STDOUT_FILENO);
			if (input_fd != STDIN_FILENO)
				close(input_fd);
			if (cmd_chain->next)
			{
				close(pipe_fds[0]);
				close(pipe_fds[1]);
			}
			child_routine(cmd_chain->simple_command, shell);
		}
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		if (cmd_chain->next)
		{
			close(pipe_fds[1]);
			input_fd = pipe_fds[0];
		}
		cmd_chain = cmd_chain->next;
	}
	wait_for_children(last_pid, shell);
}
