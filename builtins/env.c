/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:08:27 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 23:05:31 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **args, t_shell *shell)
{
	t_list	*current_node;
	t_env	*current_env;

	if (args[1] != NULL)
	{
		ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
		return (1);
	}
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
