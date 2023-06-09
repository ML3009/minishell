#include "../../include/minishell.h"
static bool has_quotes(char *str);

int pass_quotes(char *str, int i)
{
	char c;

	c = str[i];
	i++;
	while(str[i] != c && str[i] != '\0')
		i++;
	return (i);
}

int end_of_quotes(char *str, int i)
{
	char c;

	c = str[i];
	i++;
	while (str[i] != c && str[i] != '\0')
		i++;
	while ((str[i] != '&' && str[i+1] != '&') && (str[i] != '|' && str[i+1] != '|') && str[i] != '\0')
	{
		if ((str[i] == '\'') || (str[i] == '\"'))
			i = pass_quotes(str, i);
		else
			i++;
	}
	return (i);
}

int	find_next_quotes (char *str, int pos)
{
	pos++;
	while (str[pos] != '\0')
	{
		if (str[pos] == '\"')
			return(pos);
		pos++;
	}
	return (-1);
}

char **removes_quotes(char **tab)
{
	int i;
	int j;
	int k;
	char *str;

	i = 0;
	j = 0;
	k = 0;
	while (tab[i])
	{
		if (has_quotes(tab[i]))
		{
			str = ft_malloc(ft_strlen(tab[i]) - 1, "char");
			while(tab[i][j])
			{
				if (!(tab[i][j] == '\'' || tab[i][j] == '\"'))
				{
					str[k] = tab[i][j];
					k++;
					j++;
				}
				else
				{
					if (tab[i][j+1])
						j++;
					else
					{
						str[k] = '\0';
						break;
					}
				}
			}
			tab[i] = str;
		}
		i++;
	}
	return(tab);
}

static bool has_quotes(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if (str[i]== '\'' || str[i] == '\"')
			return true;
		i++;
	}
	return false;
}