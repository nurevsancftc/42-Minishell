/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:16:08 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 23:18:22 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_target_path(char **args, t_shell *shell, char **path)
{
	if (args[1] == NULL || strcmp(args[1], "~") == 0)
	{
		*path = get_env_value(shell->env_list, "HOME");
		if (*path == NULL)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		*path = get_env_value(shell->env_list, "OLDPWD");
		if (*path == NULL)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			return (1);
		}
		ft_putstr_fd(*path, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	else
		*path = args[1];
	return (0);
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*target_path;
	char	current_pwd_buffer[PATH_MAX];

	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (getcwd(current_pwd_buffer, PATH_MAX) == NULL)
	{
		perror("minishell: cd");
		return (1);
	}
	if (get_target_path(args, shell, &target_path) != 0)
		return (1);
	if (chdir(target_path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	update_or_create_env(shell, "OLDPWD", current_pwd_buffer);
	if (getcwd(current_pwd_buffer, PATH_MAX) != NULL)
		update_or_create_env(shell, "PWD", current_pwd_buffer);
	return (0);
}
