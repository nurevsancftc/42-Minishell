/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:38:45 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/21 01:23:42 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *key)
{
	int	i;

	i = 0;
	if (!key || (!ft_isalpha(key[i]) && key[i] != '_'))
		return (0);
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			if (key[i] == '+' && key[i + 1] == '\0')
				return (1);
			return (0);
		}
		i++;
	}
	return (1);
}

static char	*append_normal_text(char *line, int *i, int j, char *final_str)
{
	char	*temp_str;
	char	*new_final;

	temp_str = ft_substr(line, *i, j - *i);
	new_final = ft_strjoin(final_str, temp_str);
	free(final_str);
	free(temp_str);
	*i = j;
	return (new_final);
}

static char	*append_variable(char *line, int *i, \
	char *final_str, t_shell *shell)
{
	const char	*line_ptr;
	char		*var_value;
	char		*new_final;

	line_ptr = &line[*i];
	var_value = handle_dollar_sign(&line_ptr, shell);
	new_final = ft_strjoin(final_str, var_value);
	free(final_str);
	free(var_value);
	final_str = new_final;
	*i = line_ptr - line;
	return (final_str);
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	char	*final_str;
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
			final_str = append_normal_text(line, &i, j, final_str);
		if (line[j] == '$')
			final_str = append_variable(line, &i, final_str, shell);
		else
			i = j;
	}
	return (final_str);
}
