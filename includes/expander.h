/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:07:07 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 18:13:26 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"

void	expander(t_command_chain *cmd_chain, t_shell *shell);
void	expand_simple_command(t_simple_command *cmd, t_shell *shell);
char	*expand_word(const char *word, t_shell *shell);
void	expand_redirections(t_simple_command *cmd, t_shell *shell);
t_list	*array_to_list(char **array);
char	**list_to_array(t_list *list_head);
char	*ft_strunquote(const char *str);

#endif