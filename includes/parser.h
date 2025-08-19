/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 03:33:33 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 17:54:28 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF,
}	t_token_type;

typedef enum e_path_status
{
	PATH_VALID_FILE,
	PATH_IS_DIRECTORY,
	PATH_NOT_FOUND,
	PATH_NO_PERMISSION
} t_path_status;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
}	t_env;


typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	int				expand_in_heredoc;
}	t_redir;

typedef struct s_simple_command
{
	char	**args;
	t_list	*redirections;
}	t_simple_command;

typedef struct s_command_chain
{
	t_simple_command		*simple_command;
	struct s_command_chain	*next;
}	t_command_chain;

typedef struct s_shell
{
	t_list	*env_list;
	int		exit_code;
	t_command_chain	*cmd_tree;
}	t_shell;

t_list	*create_env_list(char **envp);
void	main_loop(t_shell *shell);


// lexer.c
t_token	*lexer(const char *line);

// quotes.c
int		handle_quotes(const char *line, t_token **token_list);

// parser_utils.c
void	add_token_to_list(t_token **list_head, t_token *new_token);
t_token	*create_token(char *value, t_token_type type);
void	free_token_list(t_token *head);
int		is_metachar(char c);
int		is_whitespace(char c);
void free_args(char **args);

// redirection.c
void free_token_list(t_token *head);
void free_cmd_tree(t_command_chain *head);
void	free_env_content(void *content);


t_command_chain	*parser(t_token *tokens);
int	populate_simple_cmd(t_simple_command *cmd, t_token **token_cursor);


void	expander(t_command_chain *cmd_chain, t_shell *shell);
void	expand_simple_command(t_simple_command *cmd, t_shell *shell);
char	*expand_word(const char *word, t_shell *shell);
void	expand_redirections(t_simple_command *cmd, t_shell *shell); //

t_list	*array_to_list(char **array);
char	**list_to_array(t_list *list_head);

char	*ft_strunquote(const char *str);

#endif