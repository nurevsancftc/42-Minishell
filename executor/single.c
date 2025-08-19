/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_single.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:29:15 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/19 22:29:20 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_dispatch(char **args, t_shell *shell)
{
	int	exit_status;

	exit_status = execute_builtin(args, shell);
	if (exit_status != SHELL_SHOULD_EXIT)
	{
		shell->exit_code = exit_status;
		return (0);
	}
	return (SHELL_SHOULD_EXIT);
}

static void	update_status_from_wait(int status, t_shell *shell)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			g_status = STATUS_CTRL_C;
			shell->exit_code = 130;
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			g_status = STATUS_QUIT;
			shell->exit_code = 131;
		}
		else
			shell->exit_code = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
}

int	execute_single_command(t_command_chain *chain_node, t_shell *shell)
{
	int					original_fds[2];
	t_simple_command	*cmd;
	int					rc;

	if (!chain_node || !chain_node->simple_command)
		return (0);
	cmd = chain_node->simple_command;
	if (handle_redirections(cmd, original_fds) == -1)
		return (shell->exit_code = 1, 0);
	if (!cmd->args || !cmd->args[0])
		return (restore_fds(original_fds), 0);
	if (is_builtin(cmd->args[0]))
	{
		rc = builtin_dispatch(cmd->args, shell);
		if (rc == SHELL_SHOULD_EXIT)
			return (restore_fds(original_fds), SHELL_SHOULD_EXIT);
	}
	else
		shell->exit_code = execute_external_command(cmd, shell, chain_node);
	restore_fds(original_fds);
	return (0);
}