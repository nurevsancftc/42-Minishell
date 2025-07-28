#ifndef MINISHELL_H
# define MINISHELL_H

# include "parser.h"
# include "../42-Minishell/libft/libft.h"

// typedef struct s_env
// {
// 	char			*key;
// 	char			*value;
// 	struct s_env	*next;
// }	t_env;

typedef struct s_shell
{
	t_list	*env_list;
	int		exit_code;
}	t_shell;

typedef struct s_redir
{
	t_token_type	type;       // Yönlendirme tipi (T_REDIR_OUT, vb.)
	char			*filename;  // Yönlendirilecek dosyanın adı
}	t_redir;

typedef struct s_simple_cmd
{
	char	**args;      // Komut ve argümanları (execve'e uygun)
	t_list	*redirections; // t_redir'lerden oluşan bir liste   //LİBFT DEKİ LİST T_LİST
}	t_simple_cmd;

typedef struct s_cmd_table
{
	t_simple_cmd		*simple_cmd; // Bu düğümdeki basit komut
	struct s_cmd_table	*next;       // Pipe ile sonraki komuta işaretçi
}	t_cmd_table;

// typedef struct s_command
// {
// 	char			**args;
// 	char			*redir_in;
// 	char			*redir_out;
// 	int				append_mode;
// 	char			*heredoc_eof;
// 	struct s_cmd	*next;

// }	t_command;

#endif