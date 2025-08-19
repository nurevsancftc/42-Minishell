/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:49:43 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/20 01:47:26 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_array(char **split_array)
{
	int	i;

	if (!split_array)
		return ;
	i = 0;
	while (split_array[i])
	{
		free(split_array[i]);
		i++;
	}
	free(split_array);
}

static char	*try_each_path(char *cmd, char **paths)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = -1;
	while (paths && paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

char	*get_command_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*path_var;
	char	*executable_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_var = get_env_value(shell->env_list, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	executable_path = try_each_path(cmd, paths);
	if (paths)
		ft_free_array(paths);
	return (executable_path);
}

static int	populate_env_array(char **env_array, t_list *env_list)
{
	t_env	*env;
	char	*temp;
	int		i;

	i = 0;
	while (env_list)
	{
		env = (t_env *)env_list->content;
		if (env->value)
		{
			temp = ft_strjoin(env->key, "=");
			if (!temp)
				return (-1);
			env_array[i] = ft_strjoin(temp, env->value);
			free(temp);
			if (!env_array[i])
				return (-1);
			i++;
		}
		env_list = env_list->next;
	}
	env_array[i] = NULL;
	return (0);
}

static int	count_valid_env_vars(t_list *env_list)
{
	int		count;
	t_env	*env;

	count = 0;
	while (env_list)
	{
		env = (t_env *)env_list->content;
		if (env->value)
			count++;
		env_list = env_list->next;
	}
	return (count);
}
