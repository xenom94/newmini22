/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/11/16 22:29:51 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_command_execution(t_command *current, char **env)
{
	if (current->redirections)
		setup_redirection(current);
	if (is_builtin(current) != NOT_BUILT_IN)
		execute_builtin(current, env, is_builtin(current));
	else
		execute_external_command(current, env);
	exit(0);
}

void	wait_for_children(pid_t *pids, int pipe_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipe_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == pipe_count - 1)
		{
			if (WIFEXITED(status))
				g_vars.exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				g_vars.exit_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGQUIT)
					write(1, "Quit\n", 5);
			}
		}
		i++;
	}
}

void	handle_pipes(t_command *commands, char **env)
{
	t_pipe_data	data;
	int			prev_pipe[2];
	int			curr_pipe[2];
	pid_t		pid;

	if (!initialize_pipe_data(&data, commands))
		return ;
	pipe_signals();
	data.current = commands;
	data.i = 0;
	while (data.current)
	{
		if (!setup_next_pipe(curr_pipe, data.current))
			return ;
		printf("pipe: %d %d\n", curr_pipe[0], curr_pipe[1]);
		pid = fork();
		if (pid == 0)
			setup_child_process(prev_pipe, curr_pipe, data.current, env);
		data.pids[data.i++] = pid;
		close_prev_pipe(prev_pipe);
		update_prev_pipe(prev_pipe, curr_pipe);
		data.current = data.current->next;
	}
	wait_for_children_and_cleanup(&data);
	all_signals();
}