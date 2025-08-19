/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:18:20 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 23:19:32 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H
#include <signal.h>

extern volatile sig_atomic_t    g_status;

// --- Global Durum Kodları (MACROS) ---
# define STATUS_OK 0              // Normal çalışma
# define STATUS_CTRL_C 1          // İnteraktif modda Ctrl+C basıldı
# define STATUS_HEREDOC_CTRL_C 2  // Heredoc sırasında Ctrl+C basıldı
# define STATUS_QUIT 3            // Ctrl+\ (SIGQUIT) basıldı

# define MODE_INTERACTIVE 0
# define MODE_CHILD       1
# define MODE_PARENT      2

void    setup_signals(int mode);
void    heredoc_signal_handler(int signo);
void    interactive_signal_handler(int signo);
void    parent_signal_handler(int signo);

// void init_signals(void);
// void heredoc_signal_handler(int signo);
// void interactive_signal_handler(int signo);
// void parent_signal_handler(int signo);
// void setup_interactive_signals(void);
// void setup_child_signals(void);
// void setup_parent_signals(void);

#endif