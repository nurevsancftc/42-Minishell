/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:58:42 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/05 18:24:04 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * expand_word - Tek bir kelimeyi (argüman veya dosya adı) genişletir.
 * Değişkenleri değiştirir ve tırnakları kaldırır.
 */
char	*expand_word(const char *word, t_shell *shell)
{
	char	*new_word; // Genişletilmiş yeni kelimeyi burada biriktireceğiz
	char	quote_type; // ' veya " veya 0 (tırnak içinde değil)

	new_word = ft_strdup(""); // Boş bir string ile başla
	quote_type = 0;

	while (*word)
	{
		if (quote_type == 0 && (*word == '\'' || *word == '"'))
		{
			// Tırnak moduna gir
			quote_type = *word;
			word++;
		}
		else if (quote_type != 0 && *word == quote_type)
		{
			// Tırnak modundan çık
			quote_type = 0;
			word++;
		}
		else if (quote_type != '\'' && *word == '$')
		{
			// Değişken genişletme (tek tırnak içinde değilse)
			char *expanded_var = handle_dollar_sign(&word, shell);
			char *temp = ft_strjoin(new_word, expanded_var);
			free(new_word);
			free(expanded_var);
			new_word = temp;
		}
		else
		{
			// Normal bir karakter, yeni kelimeye ekle
			char temp_str[2] = {*word, '\0'};
			char *temp = ft_strjoin(new_word, temp_str);
			free(new_word);
			new_word = temp;
			word++;
		}
	}
	return (new_word);
}

/**
 * expand_simple_command - Tek bir komutun argümanlarını ve yönlendirmelerini genişletir.
 */
void	expand_simple_command(t_simple_command *cmd, t_shell *shell)
{
	t_list	*arg_list;
	t_list	*current_node;
	char	*original_word;

	// 1. ADIM: Mevcut char**'ı geçici bir listeye dönüştür.
	arg_list = array_to_list(cmd->args);

	// 2. ADIM: Liste üzerinde genişletme yap.
	current_node = arg_list;
	while (current_node)
	{
		original_word = (char *)current_node->content;
		current_node->content = expand_word(original_word, shell);
		free(original_word); // Eski, genişletilmemiş kelimeyi sil.
		current_node = current_node->next;
	}

	// Yönlendirmeleri genişletme kısmı aynı kalabilir...
	// ...

	// 3. ADIM: Orijinal char**'ı temizle ve yerine yeni, genişletilmiş olanı koy.
	free_args(cmd->args); // free_array sizin `char**` temizleme fonksiyonunuz olmalı
	cmd->args = list_to_array(arg_list);

	// 4. ADIM: Geçici listeyi temizle.
	ft_lstclear(&arg_list, free);
}

/**
 * expander - Komut ağacını gezer ve her bir komut düğümünü genişletir.
 */
void	expander(t_command_chain *cmd_chain, t_shell *shell)
{
	while (cmd_chain)
	{
		expand_simple_command(cmd_chain->simple_command, shell);
		cmd_chain = cmd_chain->next;
	}
}