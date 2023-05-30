#include "../../include/minishell.h"
static void parse_data(t_binary *tree, t_env *env);
static void create_root(t_binary *tree, t_env *env);
static t_binary *new_branche(t_binary *tree, char *str);

void parsing(t_minishell *mini, char *str)
{
	mini->tree = new_branche(mini->tree, str);
	while (mini->tree->end != 1)
		parse_data(mini->tree, mini->env);
}

static void parse_data(t_binary *tree, t_env *env)
{
	if (tree->left == NULL)
		create_root(tree, env);
	if (tree->command == NULL) {
			tree->end = 1;
			return;
	}
	tree->left = new_branche(tree->left, tree->command);
	tree->right = new_branche(tree->right, tree->rest);
	ft_printf ("\n\ntree left = %s |||| ", tree->left->data);
	ft_printf ("tree right = %s \n", tree->right->data);
	tree->right->prev = tree;
	tree->left->prev = tree;
	if (tree->right != NULL && tree->right->end != 1)
		parse_data(tree->right, env);
	if (tree->right->end == 1)
		tree->end = 1;
}

static void create_root(t_binary *tree, t_env *env)
{
	int split;

	if (tree->data[0] == '(')
		ignore_parentheses(tree);
	else if (is_a_meta(tree->data[0]) == true)
		split_char(tree);
	else{
		split = find_next_split(tree, env);
		if (split == -1)
			return;
		else
		{
			tree->command = ft_limited_strdup(tree->data,0,split);
			tree->rest = ft_limited_strdup(tree->data,split + 1, ft_strlen(tree->data));
		}
		return;
	}
}

static t_binary *new_branche(t_binary *tree, char *str)
{
	tree = malloc(sizeof(t_binary));
	//tree->data = NULL;
	//tree->data = ft_malloc(ft_strlen(str), "char");
	tree->data = str;
	tree->end = 0;
	tree->prev = NULL;
	tree->left = NULL;
	tree->right = NULL;
	tree->command = NULL;
	tree->rest = NULL;
	return (tree);
}



//ok tant qu'on a pas croisee un metacar on avance.
