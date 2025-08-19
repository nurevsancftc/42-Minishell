/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                           	return (shell.exit_code);
                     +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:07:56 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/13 18:07:57 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// src/main.c

#include "minishell.h"

static void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	init_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

static void	init_shell(t_shell *shell, char **envp)
{
	shell->env_list = create_env_list(envp);
	shell->exit_code = 0;
}

static void	cleanup_shell(t_shell *shell)
{
	if (shell && shell->env_list)
		ft_lstclear(&shell->env_list, free_env_content);
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	if (argc != 1)
	{
		ft_putstr_fd("minishell: does not accept arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	(void)argv;
	init_signals();
	init_shell(&shell, envp);
	main_loop(&shell);

	cleanup_shell(&shell);
	return (shell.exit_code);
}
