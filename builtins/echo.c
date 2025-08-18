/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:00:19 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 21:34:00 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_it_an_n_option(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i] != '\0')
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	if (i > 1)
		return (1);
	else
		return (0);
}

static void	print_arguments(char **args, int i)
{
	int	first_word_printed;

	first_word_printed = 0;
	while (args[i] != NULL)
	{
		if (args[i][0] == '\0')
		{
			i++;
			continue ;
		}
		if (first_word_printed)
			printf(" ");
		printf("%s", args[i]);
		first_word_printed = 1;
		i++;
	}
}

int	ft_echo(char **args, t_shell *shell)
{
	int	i;
	int	found_n_option;

	(void)shell;
	i = 1;
	found_n_option = 0;
	while (args[i] && is_it_an_n_option(args[i]))
	{
		found_n_option = 1;
		i++;
	}
	print_arguments(args, i);
	if (!found_n_option)
		printf("\n");
	return (0);
}
