/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:14:30 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/21 03:11:31 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_heredoc_signal(int signo)
{
	(void)signo;
	g_status = STATUS_HEREDOC_CTRL_C;
	write(1, "\n", 1);
	free_for_heredoc(NULL);
	exit(130);
}

void	free_for_heredoc(t_shell *head)
{
	static t_shell	*tmp;

	if (head)
	{
		tmp = head;
	}
	if (head == NULL)
	{
		cleanup_and_exit(tmp, 130);
	}
}
