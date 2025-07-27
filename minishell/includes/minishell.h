#ifndef MINISHELL_H
#define MINISHELL_H

#include "parser.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env_list;
	int		exit_code;
}	t_shell;

typedef struct s_command
{
	char			**args;
	char			*redir_in;
	char			*redir_out;
	int				append_mode;
	char			*heredoc_eof;
	struct s_cmd	*next;

}	t_command;

#endif