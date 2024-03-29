/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmessmer <wmessmer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 11:56:54 by mvautrot          #+#    #+#             */
/*   Updated: 2023/07/24 12:55:36 by wmessmer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	meta_error(char *str);

void	exec_meta( t_binary *tree, t_minishell *mini)
{
	if (ft_strcmp(tree->data, "||") == 0)
		mini_or(tree, mini);
	else if (ft_strcmp(tree->data, "&&") == 0)
		mini_and(tree, mini);
}

void	mini_or(t_binary *tree, t_minishell *mini)
{
	if (tree->prev == NULL || tree->prev->prev == NULL)
		return (mini_error_one (5));
	if (tree->prev->prev->left->cmd->exec == 1)
	{
		if (tree->prev->right->right)
			or_norme(tree);
		else
		{
			if (tree->prev->right->parentheses == false)
				tree->prev->right->cmd->exec = 1;
			else
			{
				tree->prev->right->cmd = malloc(sizeof(t_cmd));
				tree->cmd_cr = 1;
				tree->prev->right->cmd->exec = 1;
			}	
		}
		return ;
	}
	else
		exec_recu(mini, tree->prev->right);
	free(tree->data);
}

void	mini_and(t_binary *tree, t_minishell *mini)
{
	if (tree->prev == NULL || tree->prev->prev == NULL)
	{
		mini_error_one (10);
		return ;
	}
	if (tree->prev->prev->left->cmd->exec == -1)
	{
		if (tree->prev->right->right)
			and_norme(tree);
		else
		{
			if (tree->prev->right->parentheses == false)
				tree->prev->right->cmd->exec = -1;
			else
			{
				tree->prev->right->cmd = malloc(sizeof(t_cmd));
				tree->cmd_cr = 1;
				tree->prev->right->cmd->exec = -1;
			}
		}
		return ;
	}
	else
		exec_recu(mini, tree->prev->right);
}

int	verif_meta(t_binary *tree, int i)
{
	if (!(tree->data))
		return (-1);
	if ((ft_strcmp(tree->data, "&&") == 0 || ft_strcmp(tree->data, "||") == 0))
		if (verif_meta_norme(tree) == -1)
			return (-1);
	if ((!(tree->right)) && (tree->data[0] == '|' || tree->data[0] == '&'))
	{
		if (tree->data[0] == '&')
			while (tree->data[i] == '&')
				i++;
		else if (tree->data[0] == '|')
			while (tree->data[i] == '|')
				i++;
		if (i != 2)
			return (meta_error(tree->data), -1);
	}
	else if (tree->right)
	{
		if (verif_meta(tree->left, 0) == -1)
			return (-1);
		if (verif_meta(tree->right, 0) == -1)
			return (-1);
	}
	return (0);
}

static void	meta_error(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	if (i == 1 && str[0] == '|')
		mini_error_one(4);
	if (i == 2 && str[0] == '|' && (str[1] == '>' || str[1] == '<'))
		mini_error_one(4);
	if (i == 1 && str[0] == '&')
	{
		g_eoat = 2;
		send_error("minishell: syntax error near unexpected token '&'\n");
	}
	if (i > 2 && str[0] == '|' && str[1] == '|')
		mini_error_one(5);
	if (i > 2 && str[0] == '&')
		mini_error_one(10);
}
