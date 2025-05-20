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

void	redirect_io(t_context *context)
{
	if (context->order == 2)
	{
		dup2(context->io[0], STDIN_FILENO);
		dup2(context->pipe_io[1], STDOUT_FILENO);
	}
	else if (context->order == context->argc - 2)
	{
		dup2(context->pipe_io[0], STDIN_FILENO);
		dup2(context->io[1], STDOUT_FILENO);
	}
	close(context->io[0]);
	close(context->io[1]);
	close(context->pipe_io[0]);
	close(context->pipe_io[1]);
}

void	stderror_manager(char *message, int has_prerror, int exit_mode,
	t_context context)
{
	if (!message)
		return ;
	if (!has_prerror)
	{
		write(2, message, ft_strlen(message));
	}
	else
	{
		perror(message);
	}
	unlink(context.out_name);
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
