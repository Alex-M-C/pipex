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

#include "pipex.h"

/*
Error codes (erase)

-Permission denied: 126
-Command not found: 127

-No error: 0
*/

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

void	error_manager(int argc, char **env, t_context *context)
{
	int	i;

	if (argc != 5)
	{
		write(2, "Error: Wrong number of arguments\n", 33);
		exit(1);
	}
	if (!env)
	{
		write(2, "Error: Missing environment\n", 27);
		exit(1);
	}
	i = -1;
	while (context->env[++i] != NULL)
	{
		if (ft_strncmp("PATH=", context->env[i], 5) == 0)
		{
			context->path = context->env[i] + 5;
			return ;
		}
	}
	context->path = "/bin:/usr/bin";
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
			return (2);
		}
		ft_free_wa(full_cmd);
		return (1);
	}
	ft_free_wa(full_cmd);
	return (2);
}

pid_t	child_procces(char *cmd, t_context *context)
{
	pid_t		child_pid;
	int			has_error;

	child_pid = fork();
	if (child_pid == -1)
		return (-1);
	else if (child_pid == 0)
	{
		redirect_io(context);
		if (is_empty_cmd(cmd) == 1)
			return (127);
		has_error = external_cmd(cmd, context->env);
		use_cmd(cmd, context->path, context->env);
		write(2, cmd, ft_strlen(cmd));
		if (has_error == 1)
			stderror_manager(": Permission denied", 1, 126);
		stderror_manager(": Command not found", 1, 127);
	}
	return (child_pid);
}

int	main(int argc, char **argv, char **env)
{
	t_context	context;
	pid_t		last_pid;

	context.env = env;
	error_manager(argc, env, &context);
	context.io[0] = open(argv[1], O_RDONLY);
	context.io[1] = open(argv[argc - 1], O_RDWR | O_TRUNC | O_CREAT, 0644);
	pipe(context.pipe_io);
	context.order = 2;
	context.argc = argc;
	context.argv = argv;
	while (context.order < argc - 1)
	{
		last_pid = child_procces(argv[context.order], &context);
		context.order += 1;
	}
	close(context.io[0]);
	close(context.io[1]);
	close(context.pipe_io[0]);
	close(context.pipe_io[1]);
	return (get_last_status(last_pid, argc, argv));
}
