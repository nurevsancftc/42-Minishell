/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:54:53 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 21:40:38 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes(const char *line, t_token **token_list)
{
	char	quote_char;
	int		i;
	char	*content;

	quote_char = line[0];
	i = 1;
	while (line[i] && line[i] != quote_char)
		i++;
	if (line[i] == '\0')
	{
		printf("minishell: syntax error: unclosed quote\n");
		return (-1);
	}
	i++;
	content = ft_substr(line, 0, i);
	add_token_to_list(token_list, create_token(content, TOKEN_WORD));
	free(content);
	return (i);
}
