/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 01:28:57 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/21 05:59:47 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_exec_absolute(t_simple_command *cmd, \
	t_shell *shell, char **envp)
{
	char	*command;
	int		error_code;

	command = cmd->args[0];
	if (ft_strchr(command, '/'))
	{
		error_code = ft_check_path_error(command, command);
		if (error_code != 0)
		{
			ft_free_array(envp);
			cleanup_and_exit(shell, error_code);
		}
		execve(command, cmd->args, envp);
		perror(command);
		ft_free_array(envp);
		cleanup_and_exit(shell, 126);
	}
}

static void	child_exec_lookup(t_simple_command *cmd, \
	t_shell *shell, char **envp)
{
	char	*command;
	char	*path;

	command = cmd->args[0];
	if (ft_strchr(command, '/'))
		return ;
	path = get_command_path(command, shell);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free_array(envp);
		cleanup_and_exit(shell, 127);
	}
	execve(path, cmd->args, envp);
	perror(path);
	free(path);
	ft_free_array(envp);
	cleanup_and_exit(shell, 126);
}

static int	parent_wait_and_signals(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	setup_signals(MODE_INTERACTIVE);
	update_status_from_wait(status, shell);
	return (shell->exit_code);
}

int	execute_external_command(t_simple_command *cmd, t_shell *shell, \
							t_command_chain *full_chain)
{
	pid_t	pid;
	char	**envp;

	(void)full_chain;
	setup_signals(MODE_PARENT);
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
	{
		setup_signals(MODE_CHILD);
		envp = convert_env_list_to_array(shell);
		child_exec_absolute(cmd, shell, envp);
		child_exec_lookup(cmd, shell, envp);
	}
	return (parent_wait_and_signals(pid, shell));
}
