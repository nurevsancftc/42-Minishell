/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:55:03 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/20 01:27:47 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, ".") == 0)
		return (1);
	return (0);
}

static int	handle_exit_builtin(char **args, t_shell *shell)
{
	int	status;

	if (ft_strcmp(args[0], "exit") == 0)
	{
		status = ft_exit(args, shell);
		if (status != SHELL_SHOULD_EXIT)
			shell->exit_code = status;
		return (status);
	}
	return (-1);
}

int	execute_builtin(char **args, t_shell *shell)
{
	int	ret;

	if (!args || !args[0])
		return (1);
	ret = handle_exit_builtin(args, shell);
	if (ret != -1)
		return (ret);
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args, shell));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, shell));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(args, shell));
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args, shell));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args, shell));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(args, shell));
	if (ft_strcmp(args[0], ".") == 0)
		return (ft_dot(args, shell));
	return (127);
}

static void	child_cleanup_and_exit(t_shell *shell, int exit_code)
{
	if (shell)
	{
		ft_lstclear(&shell->env_list, free_env_content);
	}
	rl_clear_history();
	exit(exit_code);
}

void	execute_external_in_child(char **args, t_shell *shell)
{
	char	*cmd_path;
	char	**envp_array;

	if (!args || !args[0])
		cleanup_and_exit(shell, 0);
	cmd_path = get_command_path(args[0], shell);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cleanup_and_exit(shell, 127);
	}
	envp_array = convert_env_list_to_array(shell);
	if (!envp_array)
	{
		free(cmd_path);
		cleanup_and_exit(shell, EXIT_FAILURE);
	}
	execve(cmd_path, args, envp_array);
	perror(args[0]);
	free(cmd_path);
	ft_free_array(envp_array);
	cleanup_and_exit(shell, 126);
}
