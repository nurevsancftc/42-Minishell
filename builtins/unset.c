/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:13:00 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/20 21:44:10 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_var(const char *arg, t_shell *shell)
{
	t_list	*current;
	t_list	*prev;
	t_env	*env;

	current = shell->env_list;
	prev = NULL;
	while (current)
	{
		env = (t_env *)current->content;
		if (ft_strcmp(env->key, arg) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env_list = current->next;
			ft_lstdelone(current, free_env_content);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (is_valid_identifier(args[i]))
			remove_env_var(args[i], shell);
		i++;
	}
	return (0);
}
