/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:42:35 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 18:36:52 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_and_expand(char *line, t_shell *shell, t_token **tokens)
{
	*tokens = lexer(line);
	if (!*tokens)
	{
		shell->exit_code = 2;
		return (0);
	}
	shell->cmd_tree = parser(*tokens);
	free_token_list(*tokens);
	if (!shell->cmd_tree)
	{
		shell->exit_code = 2;
		return (0);
	}
	expander(shell->cmd_tree, shell);
	return (1);
}

static int	execute_tree(t_shell *shell)
{
	if (!shell->cmd_tree)
		return (0);
	return (executor(shell->cmd_tree, shell));
}

static int	process_line(char *line, t_shell *shell)
{
	t_token	*tokens;
	int		status;

	tokens = NULL;
	status = 0;
	shell->cmd_tree = NULL;
	if (*line)
	{
		add_history(line);
		if (parse_and_expand(line, shell, &tokens))
			status = execute_tree(shell);
	}
	free_cmd_tree(shell->cmd_tree);
	shell->cmd_tree = NULL;
	return (status);
}

void	main_loop(t_shell *shell)
{
	char	*line;
	int		status;

	while (1)
	{
		setup_signals(MODE_INTERACTIVE);
		line = readline("minishell$ ");
		if (g_status == STATUS_CTRL_C)
		{
			shell->exit_code = 130;
			g_status = STATUS_OK;
		}
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		status = process_line(line, shell);
		free(line);
		if (status == SHELL_SHOULD_EXIT)
			break ;
	}
}
