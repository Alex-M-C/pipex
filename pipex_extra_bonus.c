/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_extra_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:04:37 by aleconst          #+#    #+#             */
/*   Updated: 2025/05/20 12:04:40 by aleconst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_all(t_context *context)
{
	close(context->io[0]);
	close(context->io[1]);
	close(context->pipe_io[0]);
	close(context->pipe_io[1]);
	close(context->pipe2_io[0]);
	close(context->pipe2_io[1]);
}

void	error_str(int has_error, char *cmd)
{
	char	*no_args_cmd;

	no_args_cmd = ft_strchr(cmd, ' ');
	if (no_args_cmd)
		write(2, cmd, ft_strlen(cmd) - ft_strlen(no_args_cmd));
	else
		write(2, cmd, ft_strlen(cmd));
	if (has_error == 1)
		stderror_manager(": Permission denied", 1, 126);
	else if (has_error == 2 && ft_strchr(cmd, '/'))
		stderror_manager(": File or directory does not exist", 1, 127);
	stderror_manager(": Command not found", 1, 127);
}

void	redirect_pipe(t_context *context)
{
	if (context->cmd_order % 2 == 0)
	{
		dup2(context->pipe2_io[0], STDIN_FILENO);
		dup2(context->pipe_io[1], STDOUT_FILENO);
	}
	else
	{
		dup2(context->pipe_io[0], STDIN_FILENO);
		dup2(context->pipe2_io[1], STDOUT_FILENO);
	}
}

void	sync_pipes(t_context *context)
{
	if (context->order > 3)
	{
		if (context->cmd_order % 2 == 0)
		{
			close(context->pipe_io[0]);
			close(context->pipe_io[1]);
			if (pipe(context->pipe_io) == -1)
				stderror_manager("Error: Pipe failed", 1, 1);
		}
		else
		{
			close(context->pipe2_io[0]);
			close(context->pipe2_io[1]);
			if (pipe(context->pipe2_io) == -1)
				stderror_manager("Error: Pipe failed", 1, 1);
		}
	}
}

int	here_doc(char *delimiter)
{
	int		pipe_fd[2];
	int		n_line;
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (-1);
	n_line = 1;
	ft_printf("> ");
	line = get_next_line(STDIN_FILENO);
	while (line && !(ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& ((ft_strlen(line) == ft_strlen(delimiter)
					&& line[ft_strlen(line) - 1] != '\n')
				|| (ft_strlen(line) - 1 == ft_strlen(delimiter)
					&& line[ft_strlen(line) - 1] == '\n'))))
	{
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
		n_line++;
		ft_printf("> ");
		line = get_next_line(STDIN_FILENO);
	}
	manage_line(delimiter, line, n_line);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
