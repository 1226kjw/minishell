#include "minishell.h"

void free_cmd(t_cmd *cmd)
{
	freeall(0, cmd->arg);
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->redin)
		free(cmd->redin);
	if (cmd->redout)
		free(cmd->redout);
	if (cmd->append)
		free(cmd->append);
	if (cmd->delimit)
		free(cmd->delimit);
	if (cmd->env.env_ret)
		free(cmd->env.env_str);
}

static char	*get_key(char *line, int *i)
{
	char	buf[128];
	int		k;

	k = 0;
	if (line[*i] == '$')
		++*i;
	if (line[*i] == '{')
		++*i;
	while (ft_isalpha(line[*i]) || ft_isdigit(line[*i]) || line[*i] == '_')
		buf[k++] = line[(*i)++];
	if (line[*i] == '}')
		++*i;
	buf[k] = 0;
	return (ft_strdup(buf));
}

void	parse_var(char *buf, char *line, t_list *envl)
{
	int		i;
	int		j;
	int		k;
	char	quot;
	char	*key;
	char	*val;

	quot = 0;
	i = 0;
	j = 0;
	while (line[j])
	{
		if (!quot && (line[j] == '\'' || line[j] == '\"'))
			quot = line[j];
		else if (quot && line[j] == quot)
			quot = 0;
		if (quot != '\'' && line[j] == '$')
		{
			key = get_key(line, &j);
			val = find_value(envl, key);
			k = -1;
			while (val[++k])
				buf[i++] = val[k];
			free(key);
		}
		else
			buf[i++] = line[j++];
	}
	buf[i] = 0;
}

void	parse_red(char *buf, t_cmd *cmd)
{
	int		i;
	char	quot;
	char	**tmp;

	quot = 0;
	i = -1;
	while (buf[++i])
	{
		if (!quot && (buf[i] == '\'' || buf[i] == '\"'))
			quot = buf[i];
		else if (quot && buf[i] == quot)
			quot = 0;
		if (!quot && (buf[i] == '<' || buf[i] == '>'))
		{
			if (buf[i + 1] == buf[i])
				tmp = ft_split(buf + i + 2, " <>");
			else
				tmp = ft_split(buf + i + 1, " <>");
			if (!tmp[0])
				err_msg("parse_error!\n");
			if (buf[i] == '<')
			{
				if (buf[i + 1] == '<')
					cmd->delimit = ft_strdup(tmp[0]);
				else
					cmd->redin = ft_strdup(tmp[0]);
			}
			else
			{
				if (buf[i + 1] == '>')
					cmd->append = ft_strdup(tmp[0]);
				else
					cmd->redout = ft_strdup(tmp[0]);
			}
			if (buf[i + 1] == buf[i])
				buf[i + 1] = ' ';
			buf[i++] = ' ';
			while (buf[i] == ' ')
				i++;
			while (quot || buf[i] != ' ')
			{
				if (!quot && (buf[i] == '\'' || buf[i] == '\"'))
					quot = buf[i];
				else if (quot && buf[i] == quot)
					quot = 0;
				if (!quot && (buf[i] == '<' || buf[i] == '>'))
					err_msg("parse_error!\n");
				buf[i++] = ' ';
			}
			i--;
			free_all(tmp);
		}
	}
}

void	parse_tmp(char *line, t_cmd *cmd, t_list *envl)
{
	char		buf[65536];

	parse_var(buf, line, envl);
	parse_red(buf, cmd);
	printf("%s\n%s\n%s\n%s\n",cmd->redin, cmd->redout, cmd->append, cmd->delimit);
	cmd->arg = ft_split(buf, " ");
}
