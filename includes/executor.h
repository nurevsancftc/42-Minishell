/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:50:13 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/05 13:32:14 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

// # include "minishell.h"

/* --- Ana Executor Fonksiyonu --- */
void	executor(t_command_chain *cmd_chain, t_shell *shell);

/* --- Alt Yürütücü Fonksiyonlar --- */
int		execute_builtin(char **args, t_shell *shell);
void	execute_pipeline(t_command_chain *cmd_chain, t_shell *shell);
void	execute_external_in_child(char **args, t_shell *shell);

/* --- Yardımcı (Utility) Fonksiyonlar --- */
char	*get_command_path(char *cmd, t_shell *shell);
char	**convert_env_list_to_array(t_shell *shell);
void	ft_free_array(char **split_array); // Libft'ten gelen yardımcı

int	execute_external_command(t_simple_command *cmd, t_shell *shell, t_command_chain *full_chain);
void	execute_single_command(t_command_chain *chain_node, t_shell *shell);

int handle_redirections(t_simple_command *cmd, int original_fds[2]);
void restore_fds(int original_fds[2]);
int	handle_heredocs(t_command_chain *cmd_chain);
int	is_builtin(char *cmd);


#endif