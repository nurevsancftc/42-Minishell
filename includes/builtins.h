/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:08:06 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/19 23:49:56 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif


/*
 * Built-in fonksiyonlarının, projenin merkezi veri yapıları olan
 * t_cmd ve t_env'yi tanıyabilmesi için "minishell.h" dosyasını
 * dahil etmesi ZORUNLUDUR.
 */
// # include "minishell.h"

/* ========================================================================== */
/*                       YERLEŞİK KOMUT PROTOTİPLERİ                          */
/* ========================================================================== */

// Mevcut çalışma dizinini ekrana basar.
int		ft_pwd(char **args, t_shell *shell);

// Argümanları ekrana basar. Komut argümanlarını 'cmd' yapısından alır.
int		ft_echo(char **args, t_shell *shell);

int	ft_exit(char **args, t_shell *shell);

int	ft_cd(char **args, t_shell *shell);

int	ft_env(char **args, t_shell *shell);

int	ft_export(char **args, t_shell *shell);

int	ft_unset(char **args, t_shell *shell);

t_list	*find_env_node(t_list *env_list, const char *key);

char	*get_env_value(t_list *env_list, const char *key);

int	is_valid_identifier(const char *key);

char	*handle_dollar_sign(const char **word_ptr, t_shell *shell);

char	*expand_heredoc_line(char *line, t_shell *shell);

void	update_or_create_env(t_shell *shell, const char *key, const char *value);

int	ft_dot(char **args, t_shell *shell);
int	ft_check_path_error(const char *path, const char *cmd_name);

#endif