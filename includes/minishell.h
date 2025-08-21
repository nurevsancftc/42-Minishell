/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 01:38:27 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/21 06:07:42 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h> 
# include <dirent.h>
# include <stdbool.h>

# include "signals.h"
# include "lexer.h"
# include "parser.h"
# include "expander.h"
# include "builtins.h"
# include "executor.h"

# define SHELL_SHOULD_EXIT -1

#endif