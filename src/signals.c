/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:17:50 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 23:26:03 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>

volatile sig_atomic_t	g_status = STATUS_OK;

void	setup_signals(int mode)
{
	if (mode == MODE_INTERACTIVE)
	{
		signal(SIGINT, interactive_signal_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == MODE_CHILD)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (mode == MODE_PARENT)
	{
		signal(SIGINT, parent_signal_handler);
		signal(SIGQUIT, parent_signal_handler);
	}
}

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
