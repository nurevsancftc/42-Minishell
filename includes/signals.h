/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:18:20 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 23:51:34 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>

extern volatile sig_atomic_t	g_status;

# define STATUS_OK 0
# define STATUS_CTRL_C 1
# define STATUS_HEREDOC_CTRL_C 2
# define STATUS_QUIT 3

# define MODE_INTERACTIVE 0
# define MODE_CHILD       1
# define MODE_PARENT      2

void	setup_signals(int mode);
void	heredoc_signal_handler(int signo);
void	interactive_signal_handler(int signo);
void	parent_signal_handler(int signo);

#endif