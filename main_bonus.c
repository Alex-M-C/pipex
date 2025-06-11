/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:04:37 by aleconst          #+#    #+#             */
/*   Updated: 2025/05/20 12:04:40 by aleconst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	use_cmd(char *cmd, char *paths, char **env)
{
	char	**path;
	char	**args;
	int		i;

	path = ft_split(paths, ':');
	if (!path)
		stderror_manager(ERROR_MALLOC, 1, 1);
	i = -1;
	while (path[++i] != NULL)
	{
		args = ft_split(cmd, ' ');
		if (!args)
			return (ft_free_wa(path),
				stderror_manager(ERROR_MALLOC, 1, 1));
		create_path(&path[i], args[0]);
		if (!path[i])
			return (ft_free_wa(args), ft_free_wa(path),
				stderror_manager(ERROR_MALLOC, 1, 1));
		if (access(path[i], X_OK) == 0)
			return (execve(path[i], args, env), ft_free_wa(args),
				stderror_manager("Error: execve", 0, 127));
		ft_free_wa(args);
	}
	ft_free_wa(path);
}

/* 
	none = OK
	1 = Permision denied
	2 = Not found
*/
int	external_cmd(char *cmd, char **env)
{
	char	**full_cmd;

	full_cmd = ft_split(cmd, ' ');
	if (!full_cmd)
		stderror_manager(ERROR_MALLOC, 1, 1);
	if (ft_strncmp(cmd, ".", 1) == 0 || ft_strncmp(cmd, "/", 1) == 0)
	{
		if (access(full_cmd[0], X_OK) == 0)
		{
			execve(full_cmd[0], full_cmd, env);
			ft_free_wa(full_cmd);
			stderror_manager("Error: execve", 0, 127);
		}
		else if (access(full_cmd[0], F_OK) == 0)
		{
			ft_free_wa(full_cmd);
			return (1);
		}
	}
	ft_free_wa(full_cmd);
	return (2);
}

static pid_t	child_procces(char *cmd, t_context *context)
{
	pid_t		child_pid;
	int			has_error;
	int			i;

	i = -1;
	child_pid = fork();
	if (child_pid == -1)
		return (-1);
	else if (child_pid == 0)
	{
		redirect_io(context);
		if (is_empty_cmd(cmd) == 1)
			return (write(2, cmd, ft_strlen(cmd)),
				stderror_manager(": Command not found", 1, 127), 0);
		has_error = external_cmd(cmd, context->env);
		while (context->env[++i] != NULL && !ft_strchr(cmd, '/'))
			if (ft_strncmp("PATH=", context->env[i], 5) == 0)
				use_cmd(cmd, context->env[i] + 5, context->env);
		error_str(has_error, cmd);
	}
	return (child_pid);
}

void	io_manager(int argc, char **argv, t_context *context)
{
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6)
			stderror_manager("Error: At least 6 arguments are expected", 1, 1);
		context->io[0] = here_doc(argv[2]);
		context->io[1] = open(argv[argc - 1],
				O_RDWR | O_APPEND | O_CREAT, 0644);
		context->order = 2;
		return ;
	}
	context->io[0] = open(argv[1], O_RDONLY);
	context->io[1] = open(argv[argc - 1], O_RDWR | O_TRUNC | O_CREAT, 0644);
	context->order = 1;
}

int	main(int argc, char **argv, char **env)
{
	t_context	context;
	pid_t		last_pid;

	if (argc < 5)
		return (write(2, "Error: At least 5 arguments are expected\n", 41), 1);
	io_manager(argc, argv, &context);
	context.env = env;
	context.argc = argc;
	context.argv = argv;
	context.cmd_order = 0;
	if (pipe(context.pipe_io) == -1 || pipe(context.pipe2_io) == -1)
		stderror_manager("Error: Pipe failed", 1, 1);
	while (++context.order < argc - 1)
	{
		sync_pipes(&context);
		last_pid = child_procces(argv[context.order], &context);
		context.cmd_order += 1;
	}
	close_all(&context);
	return (get_last_status(last_pid, argc, argv));
}
