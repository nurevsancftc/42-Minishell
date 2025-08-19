/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:49:01 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 21:51:52 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*create_env_node(char *env_line)
{
	t_env	*env_node;
	char	*equals_pos;

	env_node = malloc(sizeof(t_env));
	if (!env_node)
		return (NULL);
	equals_pos = ft_strchr(env_line, '=');
	if (!equals_pos)
	{
		env_node->key = ft_strdup(env_line);
		env_node->value = NULL;
	}
	else
	{
		env_node->key = ft_substr(env_line, 0, equals_pos - env_line);
		env_node->value = ft_strdup(equals_pos + 1);
	}
	if (!env_node->key || (!env_node->value && equals_pos))
	{
		free(env_node->key);
		free(env_node->value);
		free(env_node);
		return (NULL);
	}
	return (env_node);
}

static int	add_env_to_list(t_list **env_list_head, char *env_str)
{
	t_list	*new_list_node;
	t_env	*env_node_content;

	env_node_content = create_env_node(env_str);
	if (!env_node_content)
		return (-1);
	new_list_node = ft_lstnew(env_node_content);
	if (!new_list_node)
	{
		free_env_content(env_node_content);
		return (-1);
	}
	ft_lstadd_back(env_list_head, new_list_node);
	return (0);
}

t_list	*create_env_list(char **envp)
{
	t_list	*env_list_head;
	int		i;

	env_list_head = NULL;
	i = 0;
	while (envp[i])
	{
		if (add_env_to_list(&env_list_head, envp[i]) == -1)
		{
			ft_lstclear(&env_list_head, free_env_content);
			return (NULL);
		}
		i++;
	}
	return (env_list_head);
}
