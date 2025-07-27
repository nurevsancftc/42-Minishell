/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:16:08 by nuciftci          #+#    #+#             */
/*   Updated: 2025/07/27 19:32:13 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "builtins.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

// Gerekiyorsa dışarıdan:
// char *get_env_value(const char *key, t_list *env);
// void update_env_var(const char *key, const char *value, t_list *env);

static int	get_target_path(char **args, char **path)
{
	if (!args[1] || strcmp(args[1], "~") == 0)
	{
		*path = getenv("HOME");
		if (!*path)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		*path = getenv("OLDPWD");
		if (!*path)
		{
			fprintf(stderr, "minishell: cd: OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", *path);
	}
	else
		*path = args[1];
	return (0);
}

static int	change_directory(const char *path, char *old_pwd, char *new_pwd)
{
	if (getcwd(old_pwd, PATH_MAX) == NULL)
	{
		perror("minishell: cd: getcwd");
		return (1);
	}
	if (chdir(path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	if (getcwd(new_pwd, PATH_MAX) == NULL)
		perror("minishell: cd: getcwd after change");
	return (0);
}

int	ft_cd(char **args /*, t_list *env */)
{
	char	*path;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	if (args[1] && args[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (get_target_path(args, &path))
		return (1);
	if (change_directory(path, old_pwd, new_pwd))
		return (1);
	// update_env_var("OLDPWD", old_pwd, env);
	// update_env_var("PWD", new_pwd, env);
	return (0);
}
