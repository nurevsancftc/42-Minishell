/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:19:35 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/21 01:29:25 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_append_or_create(t_shell *shell, char *key, char *value,
		int is_append)
{
	char	*old_value;
	char	*new_value;

	if (is_append)
	{
		old_value = get_env_value(shell->env_list, key);
		if (old_value)
		{
			new_value = ft_strjoin(old_value, value);
			update_or_create_env(shell, key, new_value);
			free(new_value);
		}
		else
			update_or_create_env(shell, key, value);
	}
	else
		update_or_create_env(shell, key, value);
}

static int	handle_export_argument(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	int		is_append;

	if (arg[0] == '-')
		return (print_invalid_option_error(arg));
	split_arg(arg, &key, &value, &is_append);
	if (is_valid_identifier(key) == 0)
		return (print_export_error(arg));
	handle_append_or_create(shell, key, value, is_append);
	return (0);
}

int	ft_export(char **args, t_shell *shell)
{
	int	i;
	int	final_status;
	int	arg_status;

	final_status = 0;
	i = 1;
	if (args[1] == NULL)
	{
		print_sorted_env(shell);
		return (0);
	}
	while (args[i] != NULL)
	{
		arg_status = handle_export_argument(args[i], shell);
		if (arg_status != 0)
			final_status = arg_status;
		i++;
	}
	return (final_status);
}
