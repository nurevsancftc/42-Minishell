/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:58:57 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 01:53:15 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_for_heredoc(t_shell *head)
{
	static t_shell *tmp;

	if (head)
	{
		tmp = head;
	}
	if (head == NULL)
	{
		cleanup_and_exit(tmp, 130);
	}
}


void setup_heredoc_signal(int signo)
{
	(void)signo;
	g_status = STATUS_HEREDOC_CTRL_C;
	write(2,"\n",1);
	free_for_heredoc(NULL);
	exit(130);
}
static void	child_heredoc_routine(t_redir *redir, int fd, t_shell *shell)
{
	char	*line;
	char	*expanded;

	// setup_signals(MODE_CHILD);
	signal(SIGINT,setup_heredoc_signal);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: heredoc delimited by EOF (wanted `", 2);
			ft_putstr_fd(redir->filename, 2);
			ft_putstr_fd("')\n", 2);
			break;
		}
		if (ft_strncmp(line, redir->filename, ft_strlen(line) + 1) == 0)
		{
			free(line);
			break;
		}
		if (redir->expand_in_heredoc)
		{
			expanded = expand_heredoc_line(line, shell);
			write(fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
		else
			write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	cleanup_and_exit(shell, 0);
}

static int	process_single_heredoc(t_redir *redir, t_simple_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		fd;
	static int	counter = 0;
	char		tmp_filename_buf[64];
	char		*dup_path;
	char		*num;
	ft_bzero(tmp_filename_buf, sizeof(tmp_filename_buf));
	ft_strlcpy(tmp_filename_buf, "/tmp/minishell_heredoc_", sizeof(tmp_filename_buf));
	num = ft_itoa(counter++);
	if (!num)
		return (-1);
	ft_strlcat(tmp_filename_buf, num, sizeof(tmp_filename_buf));
	free(num);
	fd = open(tmp_filename_buf, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
		return (-1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		close(fd);
		unlink(tmp_filename_buf);
		return (-1);
	}
	if (pid == 0)
		child_heredoc_routine(redir, fd, shell);
	close(fd);
	waitpid(pid, &status, 0);
	setup_signals(MODE_INTERACTIVE); // Restore parent signal handling, burası modeparent mı olcak?

	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			unlink(tmp_filename_buf);
			shell->exit_code = 130;
			return (-1);
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putstr_fd("Quit (core dumped)\n", 2);
			unlink(tmp_filename_buf);
			shell->exit_code = 131;
			return (-1);
		}
	}
	else if (WIFEXITED(status))
	{
		// Eğer çıkış kodu 130 ise, bu bizim Ctrl+C durumumuzdur.
		if (WEXITSTATUS(status) == 130)
		{
			unlink(tmp_filename_buf); // Geçici dosyayı sil
			shell->exit_code = 130;
			return (-1); // Döngüyü kırmak için -1 döndür
		}
	}
	dup_path = ft_strdup(tmp_filename_buf);
	if (!dup_path)
		return (-1);
	free(redir->filename);
	redir->filename = dup_path;

	(void)cmd;
	return (0);
}

int	handle_heredocs(t_command_chain *chain, t_shell *shell)
{
	t_list	*lst;
	t_redir	*r;

	free_for_heredoc(shell);
	while (chain)
	{
		lst = chain->simple_command->redirections;
		while (lst)
		{
			r = (t_redir *)lst->content;
			if (r && r->type == TOKEN_HEREDOC)
			{
				if(g_status != STATUS_HEREDOC_CTRL_C)
				{
					if (process_single_heredoc(r, chain->simple_command, shell) == -1)
						return (-1);
				}
			}
			lst = lst->next;
		}
		chain = chain->next;
	}
	return (0);
}

void	cleanup_heredoc_files_after_exec(t_simple_command *cmd)
{
	t_list	*lst;
	t_redir	*r;

	if (!cmd)
		return ;
	lst = cmd->redirections;
	while (lst)
	{
		r = (t_redir *)lst->content;
		if (r && r->type == TOKEN_HEREDOC && r->filename)
			unlink(r->filename);
		lst = lst->next;
	}
}