/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/12 20:08:53 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_filename(const char *input, int *i, int len, t_token **tokens)
{
	int		start;
	char	*filename;

	start = *i;
	while (*i < len && !ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '\''
		&& input[*i] != '"')
	{
		(*i)++;
	}
	filename = ft_substr(input, start, *i - start);
	add_token(tokens, new_token(FILENAME, filename));
	free(filename);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Type: %d, Value: %s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
}

int	check_heredoc_delim(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == HEREDOC)
		{
			if (!tokens->next || (ft_strcmp(tokens->next->value, "<") == 0
					|| ft_strcmp(tokens->next->value, ">") == 0
					|| ft_strcmp(tokens->next->value, ">>") == 0
					|| ft_strcmp(tokens->next->value, "<<") == 0
					|| ft_strcmp(tokens->next->value, "|") == 0)
				|| ft_strcmp(tokens->next->value, "") == 0)
			{
				ft_putstr_fd("Error: Invalid or", 2);
				ft_putstr_fd("unmatched delimiter in herdoc\n", 2);
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}

void	free_command(t_command *cmd)
{
	t_redirection	*redir;
	t_redirection	*next;
	int				i;

	if (cmd->name)
		gc_add(0, cmd->name);
	i = 0;
	while (i < cmd->arg_count)
	{
		gc_add(0, cmd->args[i]);
		i++;
	}
	gc_add(0, cmd->args);
	redir = cmd->redirections;
	while (redir)
	{
		next = redir->next;
		gc_add(0, redir->filename);
		gc_add(0, redir);
		redir = next;
	}
	free(cmd);
}

void	free_command_list(t_command *list)
{
	t_command	*next;

	while (list)
	{
		next = list->next;
		free_command(list);
		list = next;
	}
}
