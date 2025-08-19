/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:26:58 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/19 23:33:38 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_status(int status, t_shell *shell,
								int *printed_signal_message)
{
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			*printed_signal_message = 1;
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putstr_fd("Quit (core dumped)\n", 2);
			*printed_signal_message = 1;
		}
		shell->exit_code = 128 + WTERMSIG(status);
	}
}

void	wait_for_all_children(pid_t last_pid, t_shell *shell)
{
	int	status;
	int	printed_signal_message;

	printed_signal_message = 0;
	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		handle_child_status(status, shell, &printed_signal_message);
	}
	while (wait(&status) > 0)
		handle_child_status(status, shell, &printed_signal_message);
}