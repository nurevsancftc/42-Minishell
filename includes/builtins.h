/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:08:06 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/05 16:41:07 by aldurmaz         ###   ########.fr       */
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

void	free_env_content(void *content);

int	is_valid_identifier(const char *key);

#endif