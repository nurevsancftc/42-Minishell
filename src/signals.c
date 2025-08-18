/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:17:50 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/18 19:08:33 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h> // ioctl için

// Bu global değişken, sinyal yöneticisinin ana kodla iletişim kurmasını sağlar.
volatile sig_atomic_t	g_status = STATUS_OK;

/**
 * @brief Bu, SADECE heredoc aktifken kullanılacak olan sinyal yöneticisidir.
 */
void	heredoc_signal_handler(int signo)
{
	(void)signo;
	// Durumu "Heredoc iptal edildi" olarak ayarla.
	g_status = STATUS_HEREDOC_CTRL_C;
	
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
    // write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
	rl_replace_line("", 0);
}

/**
 * @brief Kabuk boşta beklerken kullanılan standart sinyal yöneticisi.
 */
void	interactive_signal_handler(int signo)
{
	(void)signo;
	// Durumu "Ctrl+C basıldı" olarak ayarla.
	g_status = STATUS_CTRL_C;
	
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Parent process için sinyal işleyici
 */
void	parent_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_status = STATUS_CTRL_C;
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signo == SIGQUIT)
	{
		g_status = STATUS_QUIT;
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
}

/**
 * @brief Sinyalleri, kabuğun normal bekleme moduna ayarlar.
 */
void	setup_interactive_signals(void)
{
	signal(SIGINT, interactive_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Child process için varsayılan sinyal davranışını ayarlar
 */
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * @brief Parent process için sinyal davranışını ayarlar
 */
void	setup_parent_signals(void)
{
	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, parent_signal_handler);
}