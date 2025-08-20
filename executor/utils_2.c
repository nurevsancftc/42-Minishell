/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 01:30:55 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/20 01:47:07 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**convert_env_list_to_array(t_shell *shell)
{
	char	**env_array;
	int		valid_vars_count;

	valid_vars_count = count_valid_env_vars(shell->env_list);
	env_array = malloc(sizeof(char *) * (valid_vars_count + 1));
	if (!env_array)
		return (NULL);
	if (populate_env_array(env_array, shell->env_list) == -1)
	{
		ft_free_array(env_array);
		return (NULL);
	}
	return (env_array);
}

void	cleanup_and_exit(t_shell *shell, int exit_code)
{
	if (shell)
	{
		if (shell->cmd_tree)
			free_cmd_tree(shell->cmd_tree);
		if (shell->env_list)
			ft_lstclear(&shell->env_list, free_env_content);
	}
	rl_clear_history();
	exit(exit_code);
}

int	get_path_status(const char *path)
{
	DIR	*dir_ptr;

	if (access(path, F_OK) != 0)
	{
		return (PATH_NOT_FOUND);
	}
	dir_ptr = opendir(path);
	if (dir_ptr != NULL)
	{
		closedir(dir_ptr);
		return (PATH_IS_DIRECTORY);
	}
	return (PATH_VALID_FILE);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	restore_fds(int original_fds[2])
{
	dup2(original_fds[0], STDIN_FILENO);
	dup2(original_fds[1], STDOUT_FILENO);
	close(original_fds[0]);
	close(original_fds[1]);
}
