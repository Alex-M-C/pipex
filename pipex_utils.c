/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:05:31 by aleconst          #+#    #+#             */
/*   Updated: 2025/05/20 12:05:32 by aleconst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	is_empty_cmd(const char *cmd)
{
	int	i;

	if (!cmd)
		return (1);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n' && cmd[i] != '\r'
			&& cmd[i] != '\v' && cmd[i] != '\f')
			return (0);
		i++;
	}
	return (1);
}

int	get_last_status(pid_t last_pid, int argc, char **argv)
{
	int		final_status;
	int		status;
	pid_t	pid;

	pid = wait(&status);
	final_status = 1;
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status) && access(argv[argc - 1], W_OK) == 0)
				final_status = WEXITSTATUS(status);
			else
				final_status = 1;
		}
		pid = wait(&status);
	}
	return (final_status);
}

void	redirect_io(t_context *context)
{
	if (context->order == 2)
	{
		if (access(context->argv[1], F_OK | R_OK) == -1)
			stderror_manager(context->argv[1], 0, 1);
		dup2(context->io[0], STDIN_FILENO);
		dup2(context->pipe_io[1], STDOUT_FILENO);
	}
	else if (context->order == context->argc - 2)
	{
		if (access(context->argv[context->argc - 1], W_OK) == -1)
			stderror_manager(context->argv[context->argc - 1], 0, 1);
		dup2(context->pipe_io[0], STDIN_FILENO);
		dup2(context->io[1], STDOUT_FILENO);
	}
	close(context->io[0]);
	close(context->io[1]);
	close(context->pipe_io[0]);
	close(context->pipe_io[1]);
}

void	stderror_manager(char *message, int has_prerror, int exit_mode)
{
	if (has_prerror == 1)
	{
		write(2, message, ft_strlen(message));
		write(2, "\n", 1);
	}
	else
	{
		perror(message);
	}
	exit (exit_mode);
}

void	create_path(char **s1, char *s2)
{
	char	*new_string;

	new_string = ft_strjoin(*s1, "/");
	free(*s1);
	*s1 = ft_strjoin(new_string, s2);
	free(new_string);
}
