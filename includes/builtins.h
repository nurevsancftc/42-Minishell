/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:08:06 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/20 01:25:25 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

long long	ft_atoll(const char *str);
void		update_or_create_env(t_shell *shell, const char *key, \
	const char *value);
char		*get_env_value(t_list *env_list, const char *key);
char		*handle_dollar_sign(const char **word_ptr, t_shell *shell);
char		*expand_heredoc_line(char *line, t_shell *shell);
int			ft_cd(char **args, t_shell *shell);
int			ft_dot(char **args, t_shell *shell);
int			ft_echo(char **args, t_shell *shell);
int			ft_env(char **args, t_shell *shell);
int			ft_exit(char **args, t_shell *shell);
int			ft_export(char **args, t_shell *shell);
int			ft_pwd(char **args, t_shell *shell);
int			ft_unset(char **args, t_shell *shell);
int			is_valid_identifier(const char *key);
int			ft_check_path_error(const char *path, const char *cmd_name);
t_list		*find_env_node(t_list *env_list, const char *key);

#endif