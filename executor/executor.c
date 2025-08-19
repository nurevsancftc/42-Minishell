/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:26:50 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/19 22:29:13 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	executor(t_command_chain *cmd_chain, t_shell *shell)
{
	int	status;

	if (!cmd_chain)
		return (0);
	if (handle_heredocs(cmd_chain, shell) == -1)
		return (0);
	if (!cmd_chain->next)
	{
		status = execute_single_command(cmd_chain, shell);
		if (status == SHELL_SHOULD_EXIT)
			return (SHELL_SHOULD_EXIT);
	}
	else
	{
		execute_pipeline(cmd_chain, shell);
	}
	return (0);
}
