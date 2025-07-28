/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:11:57 by aldurmaz          #+#    #+#             */
/*   Updated: 2024/10/19 23:11:57 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *c, size_t len)
{
	size_t	n;

	if (*c == 0)
		return ((char *)str);
	n = ft_strlen(c);
	while (*str && n <= len)
	{
		if (*str == *c && ft_strncmp(str, c, n) == 0)
			return ((char *)str);
		++str;
		--len;
	}
	return (NULL);
}
