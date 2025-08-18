/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:13:00 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 23:24:47 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_node_from_list(const char *key_to_remove, t_shell *shell)
{
	t_list	*current;
	t_list	*previous;

	previous = NULL;
	current = shell->env_list;
	while (current != NULL)
	{
		if (strcmp(((t_env *)current->content)->key, key_to_remove) == 0)
		{
			if (previous == NULL)
				shell->env_list = current->next;
			else
				previous->next = current->next;
			ft_lstdelone(current, free_env_content);
			return;
		}
		previous = current;
		current = current->next;
	}
}

int	ft_unset(char **args, t_shell *shell)
{
	int		i;
	int		exit_status;

	i = 1;
	exit_status = 0;
	while (args[i] != NULL)
	{
		if (is_valid_identifier(args[i]) == 0)
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = 1;
		}
		else
		{
			remove_node_from_list(args[i], shell);
		}
		i++;
	}
	return (exit_status);
}
