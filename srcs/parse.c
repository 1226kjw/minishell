#include "minishell.h"

extern int		g_ret;

t_cmd	*free_next(t_cmd *cmd)
{
	t_cmd	*next;

	next = cmd->next;
	free_all(cmd->arg);
	if (cmd->cmd)
		free(cmd->cmd);
	return (next);
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

void	parse_var(char *buf, char *line, t_list *envl, t_cmd *cmd)
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
			if (line[j + 1] == '?')
			{
				val = ft_itoa(g_ret);
				j += 2;
			}
			else
			{
				key = get_key(line, &j);
				val = find_value(envl, key);
				free(key);
			}
			k = -1;
			while (val[++k])
				buf[i++] = val[k];
		}
		else
			buf[i++] = line[j++];
	}
	if (quot)
	{
		printf("parse error: quot");
		g_ret = 1;
		cmd->ret = 1;
	}
	buf[i] = 0;
}

void	parse_red(char *buf, t_cmd *cmd)
{
	int		i;
	char	quot;
	// char	**tmp;

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
				cmd->parse = ft_split(buf + i + 2, " <>");
			else
				cmd->parse = ft_split(buf + i + 1, " <>");
			if (!cmd->parse[0])
			{
				printf("minishell: syntax error near unexpected token 'newline'\n");
				cmd->ret = 1;
				g_ret = 258;
				return ;
			}
			if (buf[i] == '<')
			{
				if (buf[i + 1] == '<')
				{
					if (heredoc_all(cmd, buf, i))
						continue ;
					/*
					int		fd[2];
					pid_t	pid;

					if (pipe(fd) == -1)
					err_msg("pipe failed\n");
					pid = fork();
					if (pid < 0)
					err_msg("fork failed\n");
					if (pid == 0)
					{
						char	*line;

						close(fd[0]);
						while (1)
						{
							line = readline("> ");
							if (!line)
							{
								write(fd[1], "\0", 1);
								exit(1);
							}
							if (!ft_strcmp(line, delimiter))
							{
								free(line);
								write(fd[1], "\0", 1);
								exit(EXIT_SUCCESS);
							}
							write(fd[1], line, (int)ft_strlen(line));
							write(fd[1], "\n", 1);
							free(line);
						}
						close(fd[1]);
					}
					else
					{
						char	buffer[100 + 1];
						int		status;
						int		ret;
						struct termios	t_before;

						close(fd[1]);
						wait(&status);
						ret = WEXITSTATUS(status);
						if (ret == 1)
						{
							int		k;

							if (i != 0)
							cmd->delimit = fd[0];
							k = -1;
							while (buf[i] == '<' || buf[i] == ' ')
							buf[i++] = ' ';
							while (++k < (int)ft_strlen(tmp[0]))
							buf[i + k] = ' ';
							tcgetattr(STDIN_FILENO, &t_before);
							set_termcap(2);
							tcsetattr(STDIN_FILENO, TCSANOW, &t_before);
							continue ;
						}
						else
						{
							if (i != 0)
							cmd->delimit = fd[0];
							else
							{
								while (read(fd[0], buffer, 100))
								{
									printf("%s", buffer);
								}
								close(fd[0]);
								// seg fault?
							}
						}
					}*/
				}
				else
				{
					if (redin(cmd))
						return ;
				}
			}
			else
			{
				if (buf[i + 1] == '>')
				{
					if (redout_append(cmd, &cmd->append, &cmd->redout, O_APPEND))
						continue ;
						/*
					if (cmd->append > -1)
						close(cmd->append);
					cmd->append = open(tmp[0], O_CREAT | O_APPEND | O_WRONLY, 0644);
					if (cmd->append < 0)
					{
						g_ret = cmd->append;
						// cmd->ret = 1;
						continue ;
					}
					if (cmd->redout > -1)
					{
						close(cmd->redout);
						cmd->redout = -1;
					}
					*/
				}
				else
				{
					if (redout_append(cmd, &cmd->redout, &cmd->append, O_TRUNC))
						continue ;
					/*
					if (cmd->redout > -1)
						close(cmd->redout);
					cmd->redout = open(tmp[0], O_CREAT | O_TRUNC | O_WRONLY, 0644);
					if (cmd->redout < 0)
					{
						g_ret = cmd->redout;
						// cmd->ret = 1;
						continue ;
					}
					if (cmd->append > -1)
					{
						close(cmd->append);
						cmd->append = -1;
					}*/
				}
			}
			if (buf[i + 1] == buf[i])
				buf[i + 1] = ' ';
			buf[i++] = ' ';
			while (buf[i] == ' ')
				i++;
			while (buf[i] && (quot || buf[i] != ' '))
			{
				if (!quot && (buf[i] == '\'' || buf[i] == '\"'))
					quot = buf[i];
				else if (quot && buf[i] == quot)
					quot = 0;
				if (!quot && (buf[i] == '<' || buf[i] == '>'))
				{
					printf("parse error\n");
					cmd->ret = 1;
					g_ret = 1;
				}
				buf[i++] = ' ';
			}
			i--;
			free_all(cmd->parse);
		}
	}
}

void	parse_tmp(char *line, t_cmd *cmd, t_list *envl)
{
	char		buf[10000];

	cmd->redin = -1;
	cmd->redout = -1;
	cmd->append = -1;
	cmd->delimit = -1;
	cmd->ret = 0;
	parse_var(buf, line, envl, cmd);
	parse_red(buf, cmd);
	// printf("\n%s\n%d\n%d\n%d\n%s\n",buf,cmd->redin, cmd->redout, cmd->append, cmd->delimit);
	cmd->arg = ft_split(buf, " ");
}
