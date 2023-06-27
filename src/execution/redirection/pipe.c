#include "../../../include/minishell.h"

static int	count_pipe(t_binary *tree);

static int	count_pipe(t_binary *tree)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (tree->cmd->split_cmd[i])
	{
		if(is_a_pipe(tree->cmd->split_cmd[i]) == true)
			count++;
		i++;
	}
	return(count);
}

void	exec_pipe(t_binary *tree, t_minishell *mini)
{
	int	count;
	int	i;
	int	j;
	//int	k;
	(void)mini;
	
	i = 0;
	j = 0;
	//k = 0;
	count = count_pipe(tree);

	ft_printf ("count : %i\n", count);
	while (i < count)
	{
		tree->cmd->exec = 1;
		j = malloc_cmd_redir(mini, tree, j) + 1;
		for(int o = 0; tree->cmd->exec_cmd[o]; o++)
			ft_printf(" tree->cmd->exec_cmd[o] : %s\n", tree->cmd->exec_cmd[o]);
	
		if (pipe(tree->cmd->pipe_fd) == -1)
        	perror("pipe");
    	tree->cmd->fork = fork();
    	if (tree->cmd->fork == -1)
        	perror("fork");
		else if(tree->cmd->fork == 0)
    	{
			pipe_redir(mini, tree);
            execution_choice(tree, mini);
		}
		else
		{
			close(tree->cmd->pipe_fd[1]);
		}
		i++;	
	}
	tree->cmd->fork = fork();
	if (tree->cmd->fork == -1)
        perror("fork");
	j = malloc_cmd_redir(mini, tree, j);
	for(int o = 0; tree->cmd->exec_cmd[o]; o++)
			ft_printf(" tree->cmd->exec_cmd[o] : %s\n", tree->cmd->exec_cmd[o]);
	
	if(tree->cmd->fork == 0)
    {
			//close(tree->cmd->pipe_fd[0]);
			tree->cmd->check_pipe = false;
			if(dup2(tree->cmd->pipe_fd[0], STDIN_FILENO) == -1)
				perror("dup2");
			close(tree->cmd->pipe_fd[0]);
			close(tree->cmd->pipe_fd[1]);
			execution_choice(tree, mini);
	}
	else
	{
		//close(tree->cmd->pipe_fd[0]);
		close(tree->cmd->pipe_fd[1]);
	}
	while(wait(NULL) != -1)
                    ;

	return;

}



void	pipe_redir(t_minishell *mini, t_binary *tree)
{
	(void)mini;
	//close(tree->cmd->pipe_fd[0]);
	if(dup2(tree->cmd->pipe_fd[0], STDIN_FILENO))
		perror("dup2");
	if(dup2(tree->cmd->fd[1], STDOUT_FILENO) == -1)
			perror("dup2");
	close(tree->cmd->pipe_fd[0]);
	close(tree->cmd->pipe_fd[1]);
	return;
}
