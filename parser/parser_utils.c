/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:09:45 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/04 18:22:47 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parser/parser_utils.c

#include "minishell.h"

// Helper to free a 2D array
static void free_args(char **args)
{
    int i;

    if (!args)
        return;
    i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

// Helper to free the content of the redirections list (t_redir structs)
static void free_redir_node(void *content)
{
    t_redir *redir;

    if (!content)
        return;
    redir = (t_redir *)content;
    free(redir->filename);
    free(redir);
}


// Parser tarafından oluşturulan tüm komut ağacını serbest bırakır.
void free_cmd_tree(t_command_chain *head)
{
    t_command_chain *tmp;

    while (head)
    {
        tmp = head->next;
        if (head->simple_command)
        {
            free_args(head->simple_command->args);
            ft_lstclear(&head->simple_command->redirections, free_redir_node);
            free(head->simple_command);
        }
        free(head);
        head = tmp;
    }
}