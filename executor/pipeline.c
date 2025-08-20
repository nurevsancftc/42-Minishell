/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:48:26 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/20 21:37:00 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process_routine(t_command_chain *node, t_shell *shell,
		int *pipe_fds, int in_fd)
{
	setup_signals(MODE_CHILD);
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (node->next)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
	}
	execute_single_command_in_child(node->simple_command, shell);
}

static int	wait_for_children(pid_t last_pid, t_shell *shell)
{
	int	status;
	int	last_status;

	last_status = 0;
	waitpid(last_pid, &last_status, 0);
	update_status_from_wait(last_status, shell);
	while (wait(&status) > 0)
		;
	setup_signals(MODE_INTERACTIVE);
	return (shell->exit_code);
}

static pid_t	execute_pipeline_step(t_command_chain *chain,
		t_shell *shell, int *in_fd)
{
	int		pipe_fds[2];
	pid_t	pid;

	if (chain->next && pipe(pipe_fds) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
		child_process_routine(chain, shell, pipe_fds, *in_fd);
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (chain->next)
	{
		close(pipe_fds[1]);
		*in_fd = pipe_fds[0];
	}
	return (pid);
}

void	execute_pipeline(t_command_chain *chain, t_shell *shell)
{
	int		in_fd;
	pid_t	pid;
	pid_t	last_pid;

	in_fd = STDIN_FILENO;
	last_pid = -1;
	setup_signals(MODE_PARENT);
	while (chain)
	{
		pid = execute_pipeline_step(chain, shell, &in_fd);
		if (pid == -1)
			return ;
		last_pid = pid;
		chain = chain->next;
	}
	wait_for_children(last_pid, shell);
}
