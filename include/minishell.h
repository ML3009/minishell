/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvautrot <mvautrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 14:10:00 by mvautrot          #+#    #+#             */
/*   Updated: 2023/06/22 11:31:56 by mvautrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
 #include <sys/stat.h>
//ls -l && echo 'test > FILE' > FILE && cat FILE
//ls "-l" -la && (cat salut) || cat coucou
//ls (-l) -la && (cat salut) || cat coucou
//(ls -ls) && cat file || ls "-l -ls (test)" -la | (test) | file
//je tente " ahahah ls -l ejejej '
// est ce qu on creer un arbre binaire a partir de ce qui a ete creer initialement
//autrement dit repartir de ce qui existe et rediviser a partir des quotes

typedef struct s_binary
{
	int	end;
	char *data;
    char *rest;
    char *command;
	int visualiser;
	bool	parentheses;
	struct s_cmd *cmd;
	struct s_binary *prev;
	struct s_binary *left;
	struct s_binary *right;
	struct s_redirection	*redir;
}           t_binary;

typedef struct s_env
{
	char	*name;
	char	*data;
	struct s_env	*next;
}				t_env;

typedef	struct s_redirection
{
	char	*redir_cmd;
	char	*redir_file;
	//int	count;
	struct s_redirection	*next;

}				t_redirection;

typedef struct s_cmd
{
	char    *str;
	int    exec;
	char    **split_cmd;
	char	**redir_cmd;
	char	**exec_cmd;
    char    *path_cmd;
    pid_t   fork;
    int     fd[2];
    int     in;
    int     out;

}            t_cmd;

typedef struct s_minishell
{
	t_env	*env;
	t_binary *tree;
	char **envp;

}				t_minishell;

/*--------------------------------------- ENVIRONNEMENT -------------------------------------------*/

void add_env(t_env **env, char **envp);
void mini_export(t_env *env, char *str);

/*---------------------------------------- PARSING ------------------------------------------------*/

void mini_parse(t_minishell *mini, char *str);
void expand(t_binary *tree, t_env *env);
void parsing(t_minishell *mini, char *str);
void parse_data(t_binary *tree, t_env *env);
void create_root(t_binary *tree, t_env *env);
t_binary *new_branche(t_binary *tree, char *str);
int	find_next_quotes (char *str, int pos);

/*------------------------------------------EXECUTION----------------------------------------------*/

void exec_recu(t_minishell *mini, t_binary *tree);
char **mini_split(char *str);
void create_cmd(t_binary *tree, t_env *env);
char *cmd_recuperation(char *str, t_env *env);
void mini_or(t_binary *tree, t_minishell *mini);
void mini_and(t_binary *tree, t_minishell *mini);

void exec_cmd_redir(t_binary *tree, t_minishell *mini);
void malloc_cmd_redir(t_minishell *mini, t_binary *tree);

bool is_a_redir(char *cmd);
bool is_a_pipe(char *cmd);
t_redirection	*ft_new_redirection(char *redir, char *file);
t_redirection	*ft_last_lst_redirection(t_redirection *lst);
void	ft_add_back_lst_redirection(t_redirection **lst, t_redirection *new);

/*------------------------------------------BUILD-IN-----------------------------------------------*/
void exec_buildin(t_binary *tree, t_minishell *mini);

void mini_echo(t_binary *tree);
void mini_exit(t_minishell *mini);
void mini_pwd(t_env *env);
void mini_cd(t_env *env, t_binary *tree);

/*-------------------------------------- UTILS - ENV ----------------------------------------------*/
t_env	*ft_new_element(char *data);
t_env	*ft_last_lst(t_env *lst);
void	ft_add_back_lst(t_env **lst, t_env *new);

/* ------------------------------------ UTILS - parsing ------------------------------------------- */
void ignore_parentheses(t_binary *tree);
void replace_parentheses(t_binary *tree, t_env *env);
int end_of_quotes(char *str, int i);
bool is_a_meta(char *str, int i);
int pass_quotes(char *str, int i);
int is_a_fonction(char *str,t_env *env);
int is_a_buildin(char *str);
void split_char(t_binary *tree);
int find_next_split(t_binary *tree, t_env *env);
bool string_analyse(char *str, t_env *env);
int split_pos(char *str, int i);
int end_of_parentheses(char *str, int position);


/*------------------------------------------SIGNALS----------------------------------------------*/

void    signal_ctrlc(int sig);

void print_binary(t_binary *tree);


#endif
