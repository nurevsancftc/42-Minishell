// src/main.c

#include "minishell.h"

/**
 * @brief Ctrl+C (SIGINT) sinyali geldiğinde çalışacak olan fonksiyon.
 * 
 * Bu fonksiyon, kabuğun kapanmasını engeller. Bunun yerine:
 * 1. Yeni bir satır basar.
 * 2. Readline'a yeni bir satıra geçtiğini söyler.
 * 3. Readline'ın mevcut girdi satırını temizler.
 * 4. Komut istemini tekrar ekrana basar.
 */
static void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
/**
 * @brief Sinyal yöneticilerini programın başında kurar.
 * 
 * - SIGINT (Ctrl+C): `signal_handler` fonksiyonumuza yönlendirilir.
 * - SIGQUIT (Ctrl+\): Kabuğun kendisi çalışırken görmezden gelinir (ignore).
 */
static void	init_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	free_env_content(void *content)
{
	t_env	*env;
	// HATA AYIKLAMA: Bu fonksiyonun çağrıldığını görelim.
	// printf("DEBUG: free_env_content çağrıldı.\n"); 
	env = (t_env *)content;
	if (!env)
		return;
	free(env->key);
	free(env->value);
	free(env);
}

static void	init_shell(t_shell *shell, char **envp)
{
	shell->env_list = create_env_list(envp);
	shell->exit_code = 0;
}

static void	cleanup_shell(t_shell *shell)
{
	if (shell && shell->env_list)
		ft_lstclear(&shell->env_list, free_env_content);
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	if (argc != 1)
	{
		ft_putstr_fd("minishell: does not accept arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	(void)argv;
	init_signals();
	init_shell(&shell, envp);
	main_loop(&shell);

	// HATA AYIKLAMA: main'in sonuna ulaşıldığını görelim.
	cleanup_shell(&shell);
	return (shell.exit_code);
}
