/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:50:13 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/03 01:35:51 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

// # include "minishell.h"

/* --- Ana Executor Fonksiyonu --- */
void	executor(t_command_chain *cmd_chain, t_shell *shell);

/* --- Alt Yürütücü Fonksiyonlar --- */
int		execute_builtin(char **args, t_shell *shell);
void	execute_external(char **args, t_shell *shell);

/* --- Yardımcı (Utility) Fonksiyonlar --- */
char	*get_command_path(char *cmd, t_shell *shell);
char	**convert_env_list_to_array(t_shell *shell);
void	ft_free_split(char **split_array); // Libft'ten gelen yardımcı

#endif