/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:46:58 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 16:14:26 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_eof_warning(char *delim)
{
	ft_putstr_fd("minishell: warning: heredoc delimited by EOF (wanted `", 2);
	ft_putstr_fd(delim, 2);
	ft_putstr_fd("')\n", 2);
}

static int	is_heredoc_delimiter(char *line, char *delim)
{
	return (ft_strncmp(line, delim, ft_strlen(line) + 1) == 0);
}

static void	write_heredoc_line(t_redir *redir, char *line,
		int fd, t_shell *shell)
{
	char	*expanded;

	if (redir->expand_in_heredoc)
	{
		expanded = expand_heredoc_line(line, shell);
		write(fd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static void	read_heredoc_loop(t_redir *redir, int fd, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_eof_warning(redir->filename);
			break ;
		}
		if (is_heredoc_delimiter(line, redir->filename))
		{
			free(line);
			break ;
		}
		write_heredoc_line(redir, line, fd, shell);
		free(line);
	}
}

void	child_heredoc_routine(t_redir *redir, int fd, t_shell *shell)
{
	signal(SIGINT, setup_heredoc_signal);
	read_heredoc_loop(redir, fd, shell);
	cleanup_and_exit(shell, 0);
}
