/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmessmer <wmessmer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 12:03:23 by wmessmer          #+#    #+#             */
/*   Updated: 2023/07/24 12:28:56 by wmessmer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	wc_addback(t_wc **list, t_wc *new)
{
	t_wc	*tmp;

	if (!(*list))
		(*list) = new;
	else
	{
		tmp = (*list);
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_wc	*new_wc(char *str)
{
	t_wc	*element;

	element = (t_wc *)malloc(sizeof(t_wc));
	if (!(element))
		return (NULL);
	element->file = str;
	element->next = NULL;
	return (element);
}

bool	first_letter(char *str, char *ex)
{
	int	count;
	int	i;

	i = 0;
	count = ft_strlen(ex);
	while (count != 0)
	{
		if (str[i] != ex[i])
			return (false);
		i++;
		count--;
	}
	return (true);
}

bool	last_letter(char *str, char *ex)
{
	int	count;
	int	i;
	int	j;

	count = ft_strlen(ex);
	i = ft_strlen(ex) - 1;
	j = ft_strlen(str) - 1;
	while (count != 0)
	{
		if (str[j] != ex[i])
			return (false);
		i--;
		j--;
		count--;
	}
	return (true);
}

char	*put_in_quotes(char *begin, char *end, char *bfwc, char *afwc)
{
	char	*str;

	str = ft_strjoat(begin, ft_strdup(" '"));
	str = ft_strjoat(str, bfwc);
	str = ft_strjoat(str, ft_strdup("*"));
	str = ft_strjoat(str, afwc);
	str = ft_strjoat(str, ft_strdup("'\0"));
	str = ft_strjoat(str, end);
	return (str);
}
