#include "../../include/minishell.h"

void	execution_choice(t_binary *tree, t_minishell *mini)
{
	if (is_here_doc(tree) >= 1)
        mini_here_doc(tree->redir->redir_file, tree);
	else if (is_a_buildin(tree->cmd->exec_cmd[0]) == 1)
		exec_buildin(tree, mini);
	if ((count_redir_right(tree) >= 1) || (count_redir_right_double(tree) >= 1) || (count_redir_left(tree) >= 1))
    {
		open_file(tree);
		if (check_open(tree->cmd->in) == -1 || check_open(tree->cmd->out) == -1)
					return;
		else
			exec_send(tree, mini);
	}
	else
		exec_send(tree, mini);


	return;
}

void	execution_choice_pipe(t_binary *tree, t_minishell *mini)
{
	if (is_here_doc(tree) >= 1)
        mini_here_doc(tree->redir->redir_file, tree);
	else if (is_a_buildin(tree->cmd->exec_cmd[0]) == 1)
		exec_buildin(tree, mini);
	else
	{
		if (is_a_buildin(tree->cmd->exec_cmd[0]) != 1)
	    	execute_cmd_pipe(tree, mini);
		else
    		exec_buildin_child(tree, mini);
	}
	return;
}
