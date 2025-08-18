/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:45:07 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 21:59:46 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_spaces_and_sign(const char *str, int *sign)
{
	int	i;

	i = 0;
	*sign = 1;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	i = skip_spaces_and_sign(str, &sign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > LLONG_MAX / 10 || \
			(result == LLONG_MAX / 10 && (str[i] - '0') > LLONG_MAX % 10))
		{
			if (sign == 1)
				return (LLONG_MAX);
			else
				return (LLONG_MIN);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		i++;
	}
	if (str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args, t_shell *shell)
{
	long long	num;

	write(1, "exit\n", 5);
	if (args[1] == NULL)
		return (SHELL_SHOULD_EXIT);
	num = ft_atoll(args[1]);
	if (is_numeric(args[1]) == 0 || (num == LLONG_MAX || num == LLONG_MIN))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		shell->exit_code = 255;
		return (SHELL_SHOULD_EXIT);
	}
	if (args[2] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	shell->exit_code = (unsigned char)num;
	return (SHELL_SHOULD_EXIT);
}
