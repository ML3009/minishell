/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmessmer <wmessmer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 13:32:17 by wmessmer          #+#    #+#             */
/*   Updated: 2023/07/26 12:04:18 by wmessmer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*sie_norme(t_env *tmp, char *str)
{
	int		i;
	int		j;
	char	*add;

	j = 0;
	i = 0;
	add = NULL;
	while (tmp->data[i] != '=')
		i++;
	i++;
	add = ft_malloc((ft_strlen(tmp->data) - i), "char");
	while (tmp->data[i] != '\0')
		add[j++] = tmp->data[i++];
	add[j] = '\0';
	str = join_all_part(str, add);
	return (str);
}

char	*jap_norme(char *str, int i, char *before, char *add)
{
	int		j;
	char	*after;

	after = NULL;
	j = 0;
	after = ft_malloc((ft_strlen(str) - (i - 1)), "char");
	after[j++] = ' ';
	while (str[i] != '\0')
		after[j++] = str[i++];
	after[j] = '\0';
	str = ft_strjoin(before, add);
	free(add);
	str = ft_strjoin(str, after);
	free(after);
	return (str);
}

char	*jap_norme_2(char *str, char *af, char *bf, int i)
{
	int		j;

	while (str[i] && str[i] != '$' && (str[i] != '\'' && str[i] != '\"'))
		i++;
	if (i != 0)
		bf = ft_limited_strdup(str, 0, i - 1);
	while ((str[i] != '\0' && str[i] != ' ') || (str[i] == '\'' \
	|| str[i] == '\"' || is_letter(str[i])))
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			i = pass_quotes(str, i) + 1;
			break ;
		}
		i++;
	}
	j = i - 1;
	if (is_letter(str[i - 1]) == false && (str[i - 1] != '\'' \
	&& str[i - 1] != '\"'))
		j--;
	while (str[i] && str[i + 1] != '\0')
		i++;
	if (i != j)
		af = ft_limited_strdup(str, j + 1, i);
	return (ft_strjoat(bf, af));
}

int	expand_norme_2(t_binary *tree, int i, int quotes)
{
	if (tree->data[i] == '*')
	{
		tree->data = wildcard(tree->data, i);
		i = 0;
	}
	else if (tree->data[i] == '\'' && quotes == 0)
		i = pass_quotes(tree->data, i) + 1;
	return (i);
}
