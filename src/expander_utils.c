/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:22:25 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 22:20:26 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*array_to_list(char **array)
{
	t_list	*list_head;
	t_list	*new_node;
	int		i;

	list_head = NULL;
	i = 0;
	if (!array)
		return (NULL);
	while (array[i])
	{
		new_node = ft_lstnew(ft_strdup(array[i]));
		if (!new_node || !new_node->content)
		{
			ft_lstclear(&list_head, free);
			return (NULL);
		}
		ft_lstadd_back(&list_head, new_node);
		i++;
	}
	return (list_head);
}

char	**list_to_array(t_list *list_head)
{
	char	**array;
	int		size;
	int		i;

	size = ft_lstsize(list_head);
	array = ft_calloc(size + 1, sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (list_head)
	{
		array[i] = ft_strdup((char *)list_head->content);
		if (!array[i])
		{
			while (i > 0)
				free(array[--i]);
			free(array);
			return (NULL);
		}
		i++;
		list_head = list_head->next;
	}
	return (array);
}

int	is_originally_quoted(const char *original_word)
{
	int len;

	if (!original_word)
		return (0);
	len = ft_strlen(original_word);
	if (len >= 2)
	{
		if ((original_word[0] == '\'' && original_word[len - 1] == '\'') || \
			(original_word[0] == '"' && original_word[len - 1] == '"'))
			return (1);
	}
	return (0);
}

char	*append_expanded_var(char *new_word, const char **word_ptr, t_shell *shell)
{
	char	*expanded_var;
	char	*temp;

	expanded_var = handle_dollar_sign(word_ptr, shell);
	temp = ft_strjoin(new_word, expanded_var);
	free(new_word);
	free(expanded_var);
	return (temp);
}

char	*append_char(char *new_word, char c)
{
	char	temp_str[2] = {c, '\0'};
	char	*temp = ft_strjoin(new_word, temp_str);
	free(new_word);
	return (temp);
}
