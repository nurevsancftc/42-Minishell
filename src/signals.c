/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:17:50 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 22:37:46 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>

volatile sig_atomic_t	g_status = STATUS_OK;

void	heredoc_signal_handler(int signo)
{
	(void)signo;
	g_status = STATUS_HEREDOC_CTRL_C;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
    rl_on_new_line();
	rl_replace_line("", 0);
}

void	interactive_signal_handler(int signo)
{
	(void)signo;
	g_status = STATUS_CTRL_C;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

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

void	setup_interactive_signals(void)
{
	signal(SIGINT, interactive_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	// free_heredoc(NULL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_parent_signals(void)
{
	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, parent_signal_handler);
}
