/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:58:57 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 16:14:56 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_tmpfile(char *buf, size_t buf_size, int *fd)
{
	static int	counter = 0;
	char		*num;

	ft_bzero(buf, buf_size);
	ft_strlcpy(buf, "/tmp/minishell_heredoc_", buf_size);
	num = ft_itoa(counter++);
	if (!num)
		return (-1);
	ft_strlcat(buf, num, buf_size);
	free(num);
	*fd = open(buf, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (*fd < 0)
		return (-1);
	return (0);
}

static int	handle_heredoc_child_status(int status, char *tmp_filename_buf,
	t_shell *shell)
{
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
	else if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		unlink(tmp_filename_buf);
		shell->exit_code = 130;
		return (-1);
	}
	return (0);
}

static int	handle_heredoc_result(int status, char *tmp_filename_buf,
		t_redir *redir, t_shell *shell)
{
	char	*dup_path;

	setup_signals(MODE_INTERACTIVE);
	if (handle_heredoc_child_status(status, tmp_filename_buf, shell) == -1)
		return (-1);
	dup_path = ft_strdup(tmp_filename_buf);
	if (!dup_path)
		return (-1);
	free(redir->filename);
	redir->filename = dup_path;
	return (0);
}

static int	process_single_heredoc(t_redir *redir,
	t_simple_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		fd;
	char	tmp_filename_buf[64];

	(void)cmd;
	if (create_tmpfile(tmp_filename_buf, sizeof(tmp_filename_buf), &fd) == -1)
		return (-1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (close(fd), unlink(tmp_filename_buf), -1);
	if (pid == 0)
		child_heredoc_routine(redir, fd, shell);
	close(fd);
	waitpid(pid, &status, 0);
	return (handle_heredoc_result(status, tmp_filename_buf, redir, shell));
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
				if (g_status != STATUS_HEREDOC_CTRL_C)
				{
					if (process_single_heredoc(r, chain->simple_command,
							shell) == -1)
						return (-1);
				}
			}
			lst = lst->next;
		}
		chain = chain->next;
	}
	return (0);
}
