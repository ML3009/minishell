/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmessmer <wmessmer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 11:24:12 by mvautrot          #+#    #+#             */
/*   Updated: 2023/07/26 14:05:58 by wmessmer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	initialize_pipe_value(t_binary *tree)
{
	tree->cmd->check_here_doc = 0;
	tree->cmd->is_a_redir = 0;
	tree->cmd->check_redir = 0;
	tree->cmd->pipe_tmp = 0;
	tree->cmd->check_pipe = 1;
	tree->cmd->status = 0;
	tree->cmd->count = 0;
	tree->cmd->check = 0;
}

int	count_pipe(t_binary *tree)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (tree->cmd->split_cmd[i])
	{
		if (is_a_pipe (tree->cmd->split_cmd[i]) == true)
			count++;
		i++;
	}
	return (count);
}

void	pipex(t_binary *tree, t_minishell *mini, int i, int j)
{
	tree->cmd->count = count_pipe(tree);
	while (i < tree->cmd->count)
	{
		tree->cmd->check_here_doc = 0;
		if (tree->redir)
			pipe_option(tree, 1);
		j = cmd_redir_malloc(tree, j, j, j) + 1;
		if (tree->cmd->check_redir < 0)
			return (pipe_option(tree, 1));
		pipe_reduce(tree);
		if (tree->cmd->fork_pipe == 0)
			fork_option (mini, tree, 1, 0);
		else
		{
			pipe_parent (tree, 1);
			if (tree->cmd->check_redir < 0)
				break ;
		}
		i++;
	}
	if (tree->cmd->check_redir >= 0)
		last_pipex(tree, mini, i, j);
	return ;
}

void	last_pipex(t_binary *tree, t_minishell *mini, int i, int j)
{
	i = j;
	if (tree->redir)
		pipe_option (tree, 1);
	if (g_eoat == 130)
	{
		close(tree->cmd->pipe_tmp);
		return ;
	}
	j = cmd_redir_malloc (tree, j, j, j);
	if (tree->cmd->check_redir < 0)
	{
		pipe_option (tree, 1);
		return ;
	}
	if (is_here_doc (tree) >= 1)
		pipe_option(tree, 2);
	tree->cmd->fork_pipe = fork();
	if (tree->cmd->fork_pipe == -1)
		perror ("fork");
	if (tree->cmd->fork_pipe == 0)
		fork_option (mini, tree, 0, i);
	else
		pipe_parent (tree, 0);
	wait_child (tree);
	return ;
}

void	wait_child(t_binary *tree)
{
	while (wait(&tree->cmd->status) != -1)
		;
	if (g_eoat == 131)
	{
		if (tree->cmd->in != -1 && tree->cmd->in != 0)
			close(tree->cmd->in);
		if (tree->cmd->out != -1 && tree->cmd->out != 0)
			close(tree->cmd->out);
		if (tree->cmd->check_here_doc == 1)
		{
			close(tree->cmd->pipe_tmp);
			unlink(".tmp");
		}
		return ;
	}
	if (WEXITSTATUS(tree->cmd->status) > 0)
		tree->cmd->exec = -1;
	else
		tree->cmd->exec = 1;
	if (tree->cmd->check_here_doc == 1)
		unlink(".tmp");
	ft_free_lst(tree->redir);
}
