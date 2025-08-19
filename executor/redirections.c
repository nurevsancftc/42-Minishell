/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:57:35 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 01:30:10 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_infile(const char *filename, t_redir_ctx *ctx)
{
	int	fd;

	if (*(ctx->in_fd) != -1)
		close(*(ctx->in_fd));
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		if (*(ctx->out_fd) != -1)
			close(*(ctx->out_fd));
		close(ctx->original_fds[0]);
		close(ctx->original_fds[1]);
		return (-1);
	}
	*(ctx->in_fd) = fd;
	return (0);
}

static int	open_outfile(const char *filename, int append, t_redir_ctx *ctx)
{
	int	fd;
	int	flags;

	if (*(ctx->out_fd) != -1)
		close(*(ctx->out_fd));
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		perror(filename);
		if (*(ctx->in_fd) != -1)
			close(*(ctx->in_fd));
		close(ctx->original_fds[0]);
		close(ctx->original_fds[1]);
		return (-1);
	}
	*(ctx->out_fd) = fd;
	return (0);
}

static int	process_redir(t_redir *r, t_redir_ctx *ctx)
{
	if (r->type == TOKEN_REDIR_IN || r->type == TOKEN_HEREDOC)
		return (open_infile(r->filename, ctx));
	if (r->type == TOKEN_REDIR_OUT)
		return (open_outfile(r->filename, 0, ctx));
	if (r->type == TOKEN_REDIR_APPEND)
		return (open_outfile(r->filename, 1, ctx));
	return (0);
}

static void	apply_final_redirs(int in_fd, int out_fd)
{
	if (in_fd != -1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

int	handle_redirections(t_simple_command *cmd, int original_fds[2])
{
	t_list		*lst;
	t_redir		*r;
	int			in_fd;
	int			out_fd;
	t_redir_ctx	ctx;

	in_fd = -1;
	out_fd = -1;
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
	if (original_fds[0] == -1 || original_fds[1] == -1)
		return (perror("minishell: dup"), -1);
	ctx.in_fd = &in_fd;
	ctx.out_fd = &out_fd;
	ctx.original_fds = original_fds;
	lst = cmd->redirections;
	while (lst)
	{
		r = (t_redir *)lst->content;
		if (r->filename && process_redir(r, &ctx) == -1)
			return (-1);
		lst = lst->next;
	}
	apply_final_redirs(in_fd, out_fd);
	return (0);
}
