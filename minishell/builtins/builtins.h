/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:08:06 by nuciftci          #+#    #+#             */
/*   Updated: 2025/07/27 16:23:18 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

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
int		ft_pwd(void);

// Argümanları ekrana basar. Komut argümanlarını 'cmd' yapısından alır.
int		ft_echo(t_cmd *cmd);



#endif