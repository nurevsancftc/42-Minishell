/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 03:33:33 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 00:04:36 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef enum e_path_status
{
	PATH_VALID_FILE,
	PATH_IS_DIRECTORY,
	PATH_NOT_FOUND,
	PATH_NO_PERMISSION
}	t_path_status;

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

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
	t_list			*env_list;
	t_command_chain	*cmd_tree;
	int				exit_code;
}	t_shell;

void			main_loop(t_shell *shell);

t_list			*create_env_list(char **envp);
void			free_env_content(void *content);

t_command_chain	*parser(t_token *tokens);

int				populate_simple_cmd(t_simple_command *cmd,
					t_token **token_cursor);

int				handle_redirection(t_token **token_cursor,
					t_simple_command *cmd);

void			free_cmd_tree(t_command_chain *head);

void			add_cmd_to_chain(t_command_chain **head,
					t_command_chain *new_node);

#endif