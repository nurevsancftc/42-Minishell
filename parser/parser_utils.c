/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:09:45 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/08 08:58:14 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parser/parser_utils.c

#include "minishell.h"

// Helper to free a 2D array
void free_args(char **args)
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
	t_redir	*redir;

	if (!content)
		return;
	
	redir = (t_redir *)content;
	
	// `redir->filename`'i serbest bırak. NULL kontrolü önemli.
	if (redir->filename)
		free(redir->filename);
	
	// `t_redir` yapısının kendisini serbest bırak.
	free(redir);
}


// Parser tarafından oluşturulan tüm komut ağacını serbest bırakır.
void free_cmd_tree(t_command_chain *head)
{
    t_command_chain		*tmp_chain;
	t_simple_command	*cmd;

    while (head)
    {
        tmp_chain = head->next;
        cmd = head->simple_command;
        if (cmd)
        {
			// ft_free_array gibi genel bir 2D dizi temizleme fonksiyonu kullan.
            ft_free_array(cmd->args); 
            
			// EN ÖNEMLİ DEĞİŞİKLİK:
			// Yönlendirme listesini, sadece `content`'i temizleyen
			// `free_redir_content` fonksiyonu ile temizle.
            if (cmd->redirections)
                ft_lstclear(&cmd->redirections, free_redir_node);
            
			free(cmd);
        }
        free(head);
        head = tmp_chain;
    }
}

char	*ft_strunquote(const char *str)
{
	int		len;
	char	quote;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	quote = str[0];
	// Eğer string en az 2 karakter uzunluğundaysa ve ilk ile son karakter
	// aynı tırnak karakteriyse...
	if (len >= 2 && (quote == '\'' || quote == '"') && str[len - 1] == quote)
	{
		// Tırnakların içini kopyala.
		return (ft_substr(str, 1, len - 2));
	}
	// Tırnak yoksa veya eşleşmiyorsa, string'in kendisinin bir kopyasını döndür.
	return (ft_strdup(str));
}