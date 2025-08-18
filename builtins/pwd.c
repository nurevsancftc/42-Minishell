/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:54:10 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/18 23:13:41 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **args, t_shell *shell)
{
	char	*current_dir;

	(void)shell;
	(void)args;
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", current_dir);
	free(current_dir);
	return (0);
}
