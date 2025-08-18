/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:15:29 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 23:27:31 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>


t_list	*find_env_node(t_list *env_list, const char *key)
{
	t_env	*current_env;

	while (env_list)
	{
		current_env = (t_env *)env_list->content;
		if (current_env && current_env->key && strcmp(current_env->key, key) == 0)
		{
			return (env_list);
		}
		env_list = env_list->next;
	}
	return (NULL);
}

char	*get_env_value(t_list *env_list, const char *key)
{
	t_list	*node;
	t_env	*env_var;

	node = find_env_node(env_list, key);
	if (node == NULL)
	{
		return (NULL);
	}
	
	env_var = (t_env *)node->content;

	if (env_var->value == NULL)
	{
		return (NULL);
	}

	return (env_var->value);
}

int	is_valid_identifier(const char *key)
{
	int i;

	i = 0;
	if (!key || !(ft_isalpha(key[i]) || key[i] == '_'))
		return (0);
	i++;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

char	*handle_dollar_sign(const char **line_ptr, t_shell *shell)
{
	const char	*start;
	char		*key;
	char		*value;

	(*line_ptr)++;

	if (**line_ptr == '?')
	{
		(*line_ptr)++;
		return (ft_itoa(shell->exit_code));
	}
	if (!ft_isalpha(**line_ptr) && **line_ptr != '_')
		return (ft_strdup("$"));

	start = *line_ptr;
	while (ft_isalnum(**line_ptr) || **line_ptr == '_')
		(*line_ptr)++;

	key = ft_substr(start, 0, *line_ptr - start);
	if (!key)
		return (ft_strdup(""));

	value = get_env_value(shell->env_list, key);
	free(key);

	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	char	*final_str;
	char	*temp_str;
	char	*var_value;
	int		i;
	int		j;

	final_str = ft_strdup("");
	i = 0;
	while (line[i])
	{
		j = i;
		while (line[j] && line[j] != '$')
			j++;

		if (j > i)
		{
			temp_str = ft_substr(line, i, j - i);
			char *new_final = ft_strjoin(final_str, temp_str);
			free(final_str);
			free(temp_str);
			final_str = new_final;
		}
		if (line[j] == '$')
		{
			const char *line_ptr = &line[j];
			var_value = handle_dollar_sign(&line_ptr, shell);
			char *new_final = ft_strjoin(final_str, var_value);
			free(final_str);
			free(var_value);
			final_str = new_final;
			i = line_ptr - line;
		}
		else
		{
			i = j;
		}
	}
	return (final_str);
}

void	update_or_create_env(t_shell *shell, const char *key, const char *value)
{
	t_list	*node;
	t_env	*env;

	node = find_env_node(shell->env_list, key);
	if (node != NULL)
	{
		env = (t_env *)node->content;
		if (value != NULL)
		{
			free(env->value);
			env->value = ft_strdup(value);
		}
	}
	else
	{
		env = ft_calloc(1, sizeof(t_env));
		if (!env)
			return;
		env->key = ft_strdup(key);
		if (value != NULL)
			env->value = ft_strdup(value);
		node = ft_lstnew(env);
		ft_lstadd_back(&(shell->env_list), node);
	}
}

int	ft_check_path_error(const char *path, const char *cmd_name)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)cmd_name, 2);
		ft_putstr_fd(": ", 2);
		perror(NULL);
		return (127);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)cmd_name, 2);
		ft_putstr_fd(": is a directory\n", 2);
		return (126);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)cmd_name, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (126);
	}
	return (0);
}