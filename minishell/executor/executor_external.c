/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_external.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:48:26 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/01 08:55:26 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

// --- Sadece bu dosyada kullanılan statik yardımcılar ---

/**
 * cleanup_after_exec - Harici komut için ayrılan belleği temizler.
 */
static void	cleanup_after_exec(char *cmd_path, char **envp_array)
{
	if (cmd_path)
		free(cmd_path);
	if (envp_array)
		ft_free_split(envp_array);
}

/**
 * child_process_routine - Çocuk sürecin görevlerini yürütür.
 * execve'yi çağırır, hata olursa programı sonlandırır.
 */
static void	child_process_routine(char *cmd_path, char **args, char **envp_array)
{
	execve(cmd_path, args, envp_array);
	perror("minishell");
	cleanup_after_exec(cmd_path, envp_array);
	exit(126);
}

/**
 * parent_process_routine - Ana sürecin görevlerini yürütür.
 * Çocuğun bitmesini bekler ve çıkış kodunu alır.
 */
static void	parent_process_routine(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
}

// --- Ana Harici Yürütücü Fonksiyonu ---

/**
 * execute_external - Bir harici komutu `fork-execve-wait` döngüsüyle çalıştırır.
 */
void	execute_external(char **args, t_shell *shell)
{
	char	*cmd_path;
	char	**envp_array;
	pid_t	pid;

	cmd_path = get_command_path(args[0], shell);
	if (!cmd_path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", args[0]);
		shell->exit_code = 127;
		return ;
	}
	envp_array = convert_env_list_to_array(shell);
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		cleanup_after_exec(cmd_path, envp_array);
		return ;
	}
	if (pid == 0)
		child_process_routine(cmd_path, args, envp_array);
	else
		parent_process_routine(pid, shell);
	cleanup_after_exec(cmd_path, envp_array);
}