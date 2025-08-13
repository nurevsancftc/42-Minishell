/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 01:38:27 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/08 07:22:56 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// # include <unistd.h>     // fork, execve, access, pid_t (genellikle)
// # include <sys/wait.h>   // waitpid, WIFEXITED, WEXITSTATUS
// # include <sys/types.h>  // pid_t (her zaman burada tanımlıdır)
// # include <stdio.h>      // perror, fprintf
// # include <stdlib.h>     // exit
// # include <readline/history.h>
// # include <readline/readline.h>
# include "../libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>


# include "parser.h"
# include "builtins.h"
# include "executor.h"

# define SHELL_SHOULD_EXIT -1

// typedef struct s_env
// {
// 	char			*key;
// 	char			*value;
// 	struct s_env	*next;
// }	t_env;

// typedef struct s_shell
// {
// 	t_list	*env_list;
// 	int		exit_code;
// }	t_shell;

// typedef struct s_redir
// {
// 	t_token_type	type;       // Yönlendirme tipi (T_REDIR_OUT, vb.)
// 	char			*filename;  // Yönlendirilecek dosyanın adı
// }	t_redir;

// typedef struct s_simple_command
// {
// 	char	**args;      // Komut ve argümanları (execve'e uygun)
// 	t_list	*redirections; // t_redir'lerden oluşan bir liste   //LİBFT DEKİ LİST T_LİST
// }	t_simple_command;

// typedef struct s_command_chain
// {
// 	t_simple_command		*simple_command; // Bu düğümdeki basit komut
// 	struct s_command_chain	*next;       // Pipe ile sonraki komuta işaretçi
// }	t_command_chain;


// t_env	*create_env_list(char **envp);
// void	main_loop(t_shell *shell);


#endif