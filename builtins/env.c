/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:08:27 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/20 22:59:13 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_env_args(char **args, t_shell *shell)
{
	char	*path;

	path = get_command_path(args[1], shell);
	if (!path)
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd("’: No such file or directory\n", 2);
		return (127);
	}
	free(path);
	ft_putstr_fd("env: ‘", 2);
	ft_putstr_fd(args[1], 2);
	ft_putstr_fd("’: Permission denied\n", 2);
	return (126);
}

int	ft_env(char **args, t_shell *shell)
{
	t_list	*current_node;
	t_env	*current_env;

	if (args[1] != NULL)
		return (handle_env_args(args, shell));
	current_node = shell->env_list;
	while (current_node != NULL)
	{
		current_env = (t_env *)current_node->content;
		if (current_env && current_env->value != NULL)
			printf("%s=%s\n", current_env->key, current_env->value);
		current_node = current_node->next;
	}
	return (0);
}
