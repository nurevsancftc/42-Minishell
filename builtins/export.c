/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:19:35 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/20 21:45:50 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static void	print_sorted_env(t_shell *shell)
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

static int	print_export_error(char *key)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(key, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (1);
}

static int	handle_export_argument(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	char	*equals_sign;

	equals_sign = ft_strchr(arg, '=');
	if (equals_sign != NULL)
	{
		*equals_sign = '\0';
		key = arg;
		value = equals_sign + 1;
	}
	else
	{
		key = arg;
		value = NULL;
	}
	if (is_valid_identifier(key) == 0)
	{
		print_export_error(arg);
		if (equals_sign)
			*equals_sign = '=';
		return (1);
	}
	if (value == NULL && find_env_node(shell->env_list, key) != NULL)
	{
		if (equals_sign)
			*equals_sign = '=';
		return (0);
	}
	update_or_create_env(shell, key, value);
	if (equals_sign)
		*equals_sign = '=';
	return (0);
}

int	ft_export(char **args, t_shell *shell)
{
	int	i;
	int	final_status;

	final_status = 0;
	i = 1;
	if (args[1] == NULL)
	{
		print_sorted_env(shell);
		return (0);
	}
	while (args[i] != NULL)
	{
		if (handle_export_argument(args[i], shell) != 0)
			final_status = 1;
		i++;
	}
	return (final_status);
}
