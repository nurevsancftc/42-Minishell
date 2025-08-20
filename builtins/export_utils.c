/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 01:28:42 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/21 01:28:39 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_sorted_env(t_shell *shell)
{
	t_list	*node;
	t_env	*env;

	node = shell->env_list;
	while (node)
	{
		env = (t_env *)node->content;
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key);
		node = node->next;
	}
}

int	print_invalid_option_error(char *arg)
{
	ft_putstr_fd("minishell: export: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": invalid option\n", STDERR_FILENO);
	return (2);
}

int	print_export_error(char *key)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(key, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (1);
}

void	split_arg(char *arg, char **key, char **value, int *is_append)
{
	char	*eq_sign;

	*is_append = 0;
	eq_sign = ft_strchr(arg, '=');
	if (eq_sign != NULL)
	{
		if (eq_sign > arg && *(eq_sign - 1) == '+')
		{
			*is_append = 1;
			*(eq_sign - 1) = '\0';
		}
		else
			*eq_sign = '\0';
		*key = arg;
		*value = eq_sign + 1;
	}
	else
	{
		*key = arg;
		*value = NULL;
	}
}
