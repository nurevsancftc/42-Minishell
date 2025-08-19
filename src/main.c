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

void	free_env_content(void *content)
{
	t_env	*env;
	env = (t_env *)content;
	if (!env)
		return;
	free(env->key);
	free(env->value);
	free(env);
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
	setup_signals(MODE_INTERACTIVE);
	init_shell(&shell, envp);
	main_loop(&shell);

	cleanup_shell(&shell);
	return (shell.exit_code);
}
