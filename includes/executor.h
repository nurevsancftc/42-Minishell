/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:50:13 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/20 01:05:52 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

void	execute_external_in_child(char **args, t_shell *shell);
void	handle_child_status(int status, t_shell *shell, \
								int *printed_signal_message);
void	wait_for_all_children(pid_t last_pid, t_shell *shell);
void	execute_pipeline(t_command_chain *cmd_chain, t_shell *shell);
void	update_status_from_wait(int status, t_shell *shell);
void	cleanup_and_exit(t_shell *shell, int exit_code);
void	restore_fds(int original_fds[2]);
void	ft_free_array(char **split_array);
char	*get_command_path(char *cmd, t_shell *shell);
char	**convert_env_list_to_array(t_shell *shell);
int		execute_builtin(char **args, t_shell *shell);
int		is_builtin(char *cmd);
int		executor(t_command_chain *cmd_chain, t_shell *shell);
int		execute_external_command(t_simple_command *cmd, t_shell *shell, \
	t_command_chain *full_chain);
int		handle_redirections(t_simple_command *cmd, int original_fds[2]);
int		execute_single_command(t_command_chain *chain_node, t_shell *shell);
int		get_path_status(const char *path);
int		ft_strcmp(const char *s1, const char *s2);
int		handle_heredocs(t_command_chain *cmd_chain, t_shell *shell);

#endif