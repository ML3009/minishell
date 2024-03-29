/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmessmer <wmessmer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 14:00:12 by mvautrot          #+#    #+#             */
/*   Updated: 2023/07/26 15:23:46 by wmessmer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	split_char(t_binary *tree)
{
	int		i;
	char	c;

	i = 0;
	c = tree->data[0];
	while (tree->data[i] == c)
		i++;
	tree->command = ft_limited_strdup(tree->data, 0, i - 1);
	if (tree->data[i] == ' ')
		i++;
	tree->rest = ft_limited_strdup(tree->data, i, ft_strlen(tree->data));
}

int	find_next_split(t_binary *tree, t_env *env)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	while (tree->data[i] != '\0')
	{
		i = ipp_norme_2(tree->data, i);
		i = fns_norme(tree->data, i);
		str = ft_limited_strdup(tree->data, j, i - 1);
		if (string_analyse(tree, str, env, 0) != true)
		{
			if (ft_strcmp(str, "|") == 0)
				tree->previous_data = 1;
			free(str);
			while ((tree->data[i] == ' ' || tree->data[i] == '\t') \
				&& tree->data[i] != '\0')
				i++;
			j = i;
		}
		else
			return (free(str), split_pos(tree, tree->data, -1));
	}
	return (-1);
}

bool	string_analyse(t_binary *tree, char *str, t_env *env, int i)
{
	if (is_a_fonction(str, env) || is_a_buildin(str))
		return (false);
	else
	{
		while (str[i])
		{
			if (str[i] == '\'' || str[i] == '\"')
				i = end_of_quotes(str, i);
			else if (str[i] == '(')
			{
				if (tree->previous_data == 1)
					return (true);
				i = end_of_parentheses(str, i);
			}
			else
			{
				if (is_a_meta(str, i) == true)
					return (true);
				i++;
			}
			if (i > (int)ft_strlen(str))
				return (false);
		}
		return (false);
	}
}

int	split_pos(t_binary *tree, char *str, int i)
{
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			i = end_of_quotes(str, i);
		else if (str[i] == '(')
		{
			if (tree->previous_data == 1)
			{
				free(str);
				return (i - 1);
			}
			i = end_of_parentheses(str, i);
		}
		else
		{
			if ((is_a_meta(str, i) == true))
			{
				if (str[0] == '&' || str[0] == '|')
					return (norme_sp(i, str, 0));
				else
					return (norme_sp(i, str, 1));
			}
		}
	}
	return (-1);
}

bool	has_nothing(char *str)
{
	int	i;

	i = 0;
	while (str && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '\0')
		return (true);
	return (false);
}
