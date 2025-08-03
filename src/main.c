// src/main.c

#include "minishell.h"

/*
 * init_shell: Programın genel durumunu tutan ana yapıyı başlatır.
 * - Ortam değişkenlerini kopyalar.
 * - Başlangıç çıkış kodunu ayarlar.
 */
void	init_shell(t_shell *shell, char **envp)
{

	(void)envp;
	// `envp` dizisini kopyalayıp kendi bağlı listemiz olan `env_list`'e aktarır.
	// Bu fonksiyonu environment.c gibi bir dosyada yazmalısın.
	shell->env_list = create_env_list(envp); 
	shell->exit_code = 0;
}

/*
 * cleanup_shell: Program kapanırken ayrılan tüm belleği serbest bırakır.
 */
void	cleanup_shell(t_shell *shell)
{
	// Ortam değişkenleri listesini temizler.
	ft_lstclear(&shell->env_list, free_env_content); // Veya özel bir free fonksiyonu
	// readline kütüphanesinin kendi geçmişini temizlemesi için
	clear_history();
}

/*
 * ANA GİRİŞ NOKTASI
 * Görevleri:
 * 1. Başlangıç kontrollerini yapmak.
 * 2. Tek seferlik kurulumları yapmak (sinyaller, ortam değişkenleri).
 * 3. Ana döngüyü çağırmak.
 * 4. Döngü bittiğinde genel temizliği yapıp çıkmak.
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	// 1. BAŞLATMA KONTROLLERİ
	// Minishell argüman kabul etmez.
	if (argc != 1)
	{
		printf("Error: minishell does not accept arguments.\n");
		return (1);
	}
	(void)argv; // Kullanılmayan değişken uyarısını engelle

	// 2. TEK SEFERLİK KURULUMLAR
	init_shell(&shell, envp); // Ortam değişkenlerini ve ana yapıyı kur
	// init_signals();           // Sinyal yöneticilerini ayarla (Ctrl+C, vb.) //YAZILMADI

	// 3. ANA DÖNGÜYÜ ÇALIŞTIR
	// Programın tüm ana işlevselliği bu fonksiyonda döner.
	// Shell'in genel durumunu (env, exit_code) parametre olarak geçeriz.
	main_loop(&shell);

	// 4. GENEL TEMİZLİK
	// `main_loop` bittiğinde (Ctrl+D veya exit komutu ile),
	// program kapanmadan önce tüm belleği temizle.
	cleanup_shell(&shell);

	return (shell.exit_code); // Son çıkış kodu ile programı sonlandır.
}