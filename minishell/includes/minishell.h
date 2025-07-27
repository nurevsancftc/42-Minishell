/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:37:48 by nuciftci          #+#    #+#             */
/*   Updated: 2025/07/27 15:40:41 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================================================================== */
/*                                KÜTÜPHANELER                                */
/* ========================================================================== */

# include <stdio.h>      // I/O fonksiyonları (printf, perror)
# include <stdlib.h>     // Bellek yönetimi (malloc, free, exit)
# include <unistd.h>     // POSIX sistem çağrıları (fork, execve, chdir, getcwd)
# include <string.h>     // String işlemleri (strcmp, strlen, strdup)
# include <sys/wait.h>   // Süreç bekleme (wait, waitpid)
# include <signal.h>     // Sinyal yönetimi (sigaction, kill)
# include <readline/readline.h> // Kullanıcı girdisi için
# include <readline/history.h>  // Komut geçmişi için

/* ========================================================================== */
/*                             MERKEZİ VERİ YAPILARI                          */
/* ========================================================================== */

/**
 * t_env: Ortam Değişkeni Düğümü
 *
 * Bu yapı, 'export', 'unset', ve 'env' gibi komutların yönettiği tek bir
 * ortam değişkenini temsil eder. Projenin başında 'envp' dizisinden okunarak
 * oluşturulan bir bağlı listenin her bir halkasıdır.
 * Örneğin, "USER=guest" değişkeni için 'key' "USER", 'value' ise "guest" olur.
 */
typedef struct s_env
{
	// Değişkenin adı ('=' işaretinden önceki kısım).
	char			*key;
	
	// Değişkenin değeri ('=' işaretinden sonraki kısım).
	char			*value;
	
	// Bu yapı bir bağlı listenin düğümüdür. Bu alan, listedeki bir
	// sonraki ortam değişkenine işaret eder. Listenin son elemanında bu
	// değer 'NULL' olur.
	struct s_env	*next;
}	t_env;


/**
 * t_cmd: Komut Yapısı
 *
 * Bu yapı, kullanıcı tarafından girilen komut satırındaki, '|' (pipe) ile
 * ayrılmış tek bir komut bloğunu temsil eder.
 *
 * ÖRNEK KOMUT ZİNCİRİ: `cat < infile.txt | grep "test" >> outfile.txt`
 * Bu komut, Parser tarafından iki adet t_cmd yapısına dönüştürülür:
 * 1. t_cmd (cat için):
 *    - args: {"cat", NULL}
 *    - infile: "infile.txt"
 *    - next: (grep komutunun t_cmd yapısına işaret eder)
 * 2. t_cmd (grep için):
 *    - args: {"grep", "test", NULL}
 *    - outfile: "outfile.txt"
 *    - append: 1
 *    - next: NULL
 */
typedef struct s_cmd
{
	// Komutun kendisini ve argümanlarını içeren 'NULL' ile sonlanan
	// string dizisi. Örnek: "wc -l" için {"wc", "-l", NULL}.
	char			**args;

	// '<' ile belirtilen girdi dosyasının adı. Yönlendirme yoksa NULL'dur.
	char			*infile;

	// '>' veya '>>' ile belirtilen çıktı dosyasının adı. Yönlendirme yoksa NULL'dur.
	char			*outfile;

	// Çıktı yönlendirmesi '>>' (ekleme modu) ile yapıldıysa 1,
	// '>' (üzerine yazma modu) ile yapıldıysa 0 olur.
	int				append;

	// '<<' (heredoc) kullanıldığında belirtilen sınırlayıcı kelime.
	// Kullanılmıyorsa NULL'dur.
	char			*heredoc_delimiter;

	// Bu komut bir '|' (pipe) ile başka bir komuta bağlıysa, bir sonraki
	// t_cmd yapısına işaret eder. Zincirin son komutuysa NULL olur.
	struct s_cmd	*next;
}	t_cmd;


/* ========================================================================== */
/*                           GENEL FONKSİYON PROTOTİPLERİ                     */
/* ========================================================================== */

// Bu alana projenin ana fonksiyonlarının (lexer, parser, executor vb.)
// prototipleri eklenecektir.


#endif