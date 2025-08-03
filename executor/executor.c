/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 05:26:50 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/03 10:22:43 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * is_single_builtin - Komut zincirinde tek bir komut olup olmadığını ve
 *                     bu komutun bir yerleşik (built-in) komut olup olmadığını
 *                     kontrol eder.
 */
static int	is_single_builtin(t_command_chain *cmd_chain)
{
	char	*cmd_name;

	if (!cmd_chain || cmd_chain->next != NULL)
		return (0);
	if (!cmd_chain->simple_command || !cmd_chain->simple_command->args)
		return (0);
	cmd_name = cmd_chain->simple_command->args[0];
	if (!cmd_name)
		return (0);
	if (strcmp(cmd_name, "cd") == 0 || strcmp(cmd_name, "exit") == 0
		|| strcmp(cmd_name, "export") == 0 || strcmp(cmd_name, "unset") == 0)
		return (1);
	return (0);
}

/**
 * executor - Komut ağacını alır ve uygun yürütme yolunu seçer.
 *
 * `cd`, `exit` gibi ana shell'i etkilemesi gereken tekil built-in'leri
 * doğrudan ana süreçte çalıştırır.
 *
 * Diğer tüm durumlar (pipe'lı komutlar, harici komutlar veya `echo`, `pwd`,
 * `env` gibi pipe içinde çalışabilen built-in'ler) için, süreçler
 * oluşturan `execute_pipeline` fonksiyonunu çağırır.
 */
void	executor(t_command_chain *cmd_chain, t_shell *shell)
{
	if (!cmd_chain || !cmd_chain->simple_command
		|| !cmd_chain->simple_command->args
		|| !cmd_chain->simple_command->args[0])
		return;
	if (is_single_builtin(cmd_chain))
	{
		execute_builtin(cmd_chain->simple_command->args, shell);
	}
	else
	{
		execute_pipeline(cmd_chain, shell);
	}
}
