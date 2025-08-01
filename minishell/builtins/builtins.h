/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:08:06 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/01 02:05:06 by nuciftci         ###   ########.fr       */
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
# include "minishell.h"

/* ========================================================================== */
/*                       YERLEŞİK KOMUT PROTOTİPLERİ                          */
/* ========================================================================== */

// Mevcut çalışma dizinini ekrana basar.
int	ft_pwd(char **args);

// Argümanları ekrana basar. Komut argümanlarını 'cmd' yapısından alır.
int	ft_echo(char **args);

int	ft_exit(char **args);

int	ft_cd(char **args /*, t_list *env */);


#endif