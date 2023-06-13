#include "../../include/minishell.h"
void exec_meta( t_binary *tree, t_minishell *mini);
void execute_cmd(t_binary *tree, char **envp);
void exec_buildin(t_binary *tree, t_minishell *mini);

void exec_recu(t_minishell *mini, t_binary *tree)
{
    char buf[6];
    int ret;

	if (tree->right)
	{
		exec_recu(mini, tree->left);
		exec_recu(mini, tree->right);
	}
	else
	{
		if (is_a_meta(tree->data[0]))
			exec_meta(tree, mini);
		else
		{
            if (tree->cmd->exec == true)
                return;
            else
            {
                if (is_a_buildin(tree->cmd->split_cmd[0]) == 1)
                    exec_buildin(tree, mini);
                else
                {
                    tree->cmd->exec = true;
                    if (pipe(tree->cmd->fd) == -1)
                        perror("pipe");
                    tree->cmd->fork = fork();
                    if (tree->cmd->fork == -1)
                        perror("fork");
                    if (tree->cmd->fork == 0)
                        execute_cmd(tree, mini->envp);
                    else
                    {
                        close(tree->cmd->fd[1]);
                        ret = read(tree->cmd->fd[0], buf, 10);
                        buf[ret] = '\0';
                        close(tree->cmd->fd[0]);
                        if (ft_strcmp(buf, "false") == 0)
                            tree->cmd->exec = false;
                        while(wait(NULL) != -1)
                            ;
                    }
                    ft_printf("exec after = %d\n",tree->cmd->exec);
                }
            }
    	}
	}
	return;
}

void exec_meta( t_binary *tree, t_minishell *mini)
{
    if (ft_strcmp(tree->data, "|") == 0)
        ft_printf("PIPE\n");
    else if (ft_strcmp(tree->data, "||")== 0)
        mini_or(tree, mini);
    else if (ft_strcmp(tree->data, "&&")== 0)
        mini_and(tree, mini);
    else if (ft_strcmp(tree->data, "<")== 0)
        ft_printf("REDIR\n");
    else if (ft_strcmp(tree->data, ">")== 0)
        ft_printf("REDIR\n");
    else if (ft_strcmp(tree->data, ">>")== 0)
        ft_printf("IDK\n");
    else if (ft_strcmp(tree->data, "<<")== 0)
        ft_printf("HERE\n");
    else
        ft_printf("error meta\n");
}

void execute_cmd(t_binary *tree, char **envp)
{
    ft_printf("exec in = %d\n",tree->cmd->exec);
    if (tree->cmd->path_cmd)
    {
        if (execve(tree->cmd->path_cmd, tree->cmd->split_cmd, envp) == -1)
        {
            close(tree->cmd->fd[0]);
            write(tree->cmd->fd[1], "false", 5);
            close(tree->cmd->fd[1]);
            ft_free_tab(tree->cmd->split_cmd);
            ft_perror(" Error : Command execution\n");
        }
    }
    else if (ft_strchr(tree->cmd->split_cmd[0], '/') != NULL)
    {
        tree->cmd->path_cmd = tree->cmd->split_cmd[0];
        if (execve(tree->cmd->path_cmd, tree->cmd->split_cmd, envp) == -1)
        {
            close(tree->cmd->fd[0]);
            write(tree->cmd->fd[1], "false", 5);
            ft_free_tab(tree->cmd->split_cmd);
            ft_perror(" Error : Command execution\n");
        }
    }
    else
    {
        close(tree->cmd->fd[0]);
        write(tree->cmd->fd[1], "false", 5);
        close(tree->cmd->fd[1]);
        ft_free_tab(tree->cmd->split_cmd);
		ft_perror("path");
    }
    ft_free_tab(tree->cmd->split_cmd);
}

void exec_buildin(t_binary *tree, t_minishell *mini)
{
    (void)tree;
    (void)mini;
    ft_printf("BI");
}