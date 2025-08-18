/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:27:32 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 20:58:23 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dot(char **args, t_shell *shell)
{
	int	error_status;

	(void)shell;
	if (args[1] == NULL || args[1][0] == '\0')
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		return (2);
	}
	error_status = ft_check_path_error(args[1], args[1]);
	if (error_status != 0)
	{
		return (1);
	}
	return (0);
}
