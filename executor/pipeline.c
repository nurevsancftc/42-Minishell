/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:48:26 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/20 01:29:56 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_io(t_command_chain *current_cmd, int pipe_fds[2],
														int input_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (current_cmd->next)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);
	}
	if (current_cmd->next)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
}

static void	child_routine(t_simple_command *cmd, t_shell *shell)
{
	int	exit_code;
	int	temp_original_fds[2];

	if (handle_redirections(cmd, temp_original_fds) == -1)
	{
		cleanup_and_exit(shell, 1);
	}
	if (cmd && cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		exit_code = execute_builtin(cmd->args, shell);
		cleanup_and_exit(shell, exit_code);
	}
	else
	{
		execute_external_in_child(cmd->args, shell);
	}
}

static pid_t	spawn_pipeline_child(t_command_chain *cmd,
					t_shell *shell, int pipe_fds[2], int input_fd)
{
	pid_t	pid;

	if (cmd->next && pipe(pipe_fds) == -1)
		return (perror("minishell: pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), -1);
	if (pid == 0)
	{
		setup_signals(MODE_CHILD);
		setup_child_io(cmd, pipe_fds, input_fd);
		child_routine(cmd->simple_command, shell);
	}
	return (pid);
}

void	execute_pipeline(t_command_chain *cmd_chain, t_shell *shell)
{
	int				pipe_fds[2];
	int				input_fd;
	pid_t			last_pid;
	t_command_chain	*current;

	input_fd = STDIN_FILENO;
	last_pid = -1;
	current = cmd_chain;
	while (current)
	{
		last_pid = spawn_pipeline_child(current, shell, pipe_fds, input_fd);
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		if (current->next)
		{
			close(pipe_fds[1]);
			input_fd = pipe_fds[0];
		}
		current = current->next;
	}
	wait_for_all_children(last_pid, shell);
}
