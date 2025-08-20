/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:13:00 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/21 00:11:19 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void remove_env_var(const char *arg, t_shell *shell)
{
	t_list  *current;
	t_list  *prev;
	t_env   *env;
	current = shell->env_list;
	prev = NULL;
	while (current)
	{
		env = (t_env *)current->content;
		if (ft_strcmp(env->key, arg) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env_list = current->next;
			ft_lstdelone(current, free_env_content);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
static int  check_unset_arg_errors(char *arg, t_shell *shell)
{
	char    *semicolon_pos;
	semicolon_pos = ft_strchr(arg, ';');
	if (semicolon_pos && *(semicolon_pos + 1) != '\0')
	{
		*semicolon_pos = '\0';
		if (is_valid_identifier(arg))
			remove_env_var(arg, shell);
		ft_putstr_fd(semicolon_pos + 1, 2);
		ft_putstr_fd(": command not found\n", 2);
		*semicolon_pos = ';';
		return (127);
	}
	if (arg[0] == '-' && arg[1] != '\0')
	{
		ft_putstr_fd("minishell: unset: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": invalid option\n", 2);
		return (2);
	}
	return (0);
}
static int  handle_unset_arg(char *arg, t_shell *shell)
{
	int error_status;
	error_status = check_unset_arg_errors(arg, shell);
	if (error_status != 0)
		return (error_status);
	if (is_valid_identifier(arg))
		remove_env_var(arg, shell);
	return (0);
}
int ft_unset(char **args, t_shell *shell)
{
	int i;
	int exit_status;
	int arg_status;
	i = 1;
	exit_status = 0;
	while (args[i])
	{
		arg_status = handle_unset_arg(args[i], shell);
		if (arg_status == 2 || arg_status == 127)
			return (arg_status);
		i++;
	}
	return (exit_status);
}