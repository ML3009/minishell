/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_norme.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvautrot <mvautrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 15:43:33 by mvautrot          #+#    #+#             */
/*   Updated: 2023/07/22 16:07:59 by mvautrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static	void	ft_wait(t_binary *tree, int status);

void	exec_recu_norme(t_minishell *mini, t_binary *tree, int i)
{
	if (i == 1)
	{
		if (tree->prev->prev->left->parentheses == false \
		&& ft_strcmp(tree->prev->prev->left->data, "||") == 0)
			ft_free_tab(tree->cmd->split_cmd);
		else if (tree->prev->left->parentheses == false \
		&& ft_strcmp(tree->prev->left->data, "||") == 0)
			ft_free_tab(tree->cmd->split_cmd);
	}
	else if (i == 2)
	{
		if (tree->status == true)
				tree->cmd->split_cmd = \
				mini_split(get_status(tree), 0, 0, 0);
			execution(mini, tree);
	}
	return;
}

void	execution_norme(t_minishell *mini, t_binary *tree, int i)
{
	if (i == 1)
	{
		g_eoat = 2;
		send_error ("minishell: syntax error near unexpected token '&'\n");
	}
	if (i == 2)
	{
		cmd_redir_malloc(tree, 0, 0, 0);
		execution_choice(tree, mini);
	}
	if (i == 3)
	{
		exec_cmd_redir(tree);
		if (is_a_buildin(tree->cmd->exec_cmd[0]) == 0)
			execute_cmd(tree, mini);
		else
			exec_buildin_child(tree, mini);
	}
	else
		return ;
}

void	execution_norme_2(t_minishell *mini, t_binary *tree, int status)
{
	tree->cmd->fork = fork();
	if (tree->cmd->fork == -1)
		perror("fork");
	else if (tree->cmd->fork == 0)
	{
		exec_cmd_redir(tree);
		if (is_a_buildin(tree->cmd->exec_cmd[0]) == 0)
			execute_cmd(tree, mini);
		else
			exec_buildin_child(tree, mini);
	}
	else
		ft_wait(tree, status);
	return ;
}

static	void	ft_wait(t_binary *tree, int status)
{
	signal(SIGQUIT, SIG_DFL);
	while (wait(&status) != -1)
		;
	g_eoat = status / 256;
	if (WEXITSTATUS(status) > 0)
		tree->cmd->exec = -1;
	if (tree->cmd->in != -1 && tree->cmd->in != 0)
		close(tree->cmd->in);
	if (tree->cmd->out != -1 && tree->cmd->out != 0)
		close(tree->cmd->out);
	if (tree->cmd->check_here_doc == 1)
	{
		close(tree->cmd->pipe_tmp);
		unlink(".tmp");
	}
}