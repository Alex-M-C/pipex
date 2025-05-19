#include "pipex.h"

void	create_path(char **s1, char *s2)
{
	char	*new_string;

	new_string = ft_strjoin(*s1, "/");
	free(*s1);
	*s1 = ft_strjoin(new_string, s2);
	free(new_string);
}

void	use_cmd(char *cmd, char *paths)
{
	char	**path;
	char	**args;
	int		i;

	path = ft_split(paths, ':');
	if (!path)
	{
		perror("Error");
		exit(1);
	}
	i = -1;
	while (path[++i] != NULL)
	{
		args = ft_split(cmd, ' ');
		if (!args)
		{
			ft_free_wa(path);
			perror("Error");
			exit(1);
		}
		create_path(&path[i], args[0]);
		if (!path[i])
		{
			perror("Error");
			ft_free_wa(args);
			ft_free_wa(path);
			exit(1);
		}
		if (access(path[i], X_OK) == 0)
			execve(path[i], args, NULL);
		ft_free_wa(args);
	}
	ft_free_wa(path);
}

void	error_manager(int argc, char **argv)
{
	if (argc != 5)
	{
		write(2, "Cantidad de argumentos incorrecta\n", 34);
		exit(1);
	}
	if (access(argv[1], F_OK | R_OK) == -1)
	{
		perror(argv[1]);
		exit(1);
	}
	if (access(argv[argc - 1], F_OK) == 0 && access(argv[argc - 1], W_OK) == -1)
	{
		perror(argv[argc - 1]);
		exit(1);
	}
}

/* 
	none = OK
	-1 = Execve failed
	1 = Permision denied
	2 = Not found
*/
int	external_cmd(char *cmd)
{
	char **full_cmd;

	full_cmd = ft_split(cmd, ' ');
	if (!full_cmd)
	{
		perror("Error ");
		exit(1);
	}
	if (ft_strncmp(cmd, ".", 1) == 0 || ft_strncmp(cmd, "/", 1) == 0)
	{	
		if (access(full_cmd[0], X_OK) == 0)
		{
			execve(full_cmd[0], full_cmd, NULL);
			perror("execve");
			ft_free_wa(full_cmd);
			exit(1);
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
	int			i;

	i = -1;
	child_pid = fork();
	if (child_pid == -1)
		return (-1);
	else if (child_pid == 0)
	{
		dup2(context->io[0], STDIN_FILENO);
		dup2(context->pipe_io[1], STDOUT_FILENO);
		close(context->io[0]);
		close(context->io[1]);
		close(context->pipe_io[0]);
		close(context->pipe_io[1]);
		has_error = external_cmd(cmd);
		while (context->env[++i] != NULL)
		{
			if (ft_strncmp("PATH=", context->env[i], 5) == 0)
				use_cmd(cmd, context->env[i] + 5);
		}
		write(2, cmd, ft_strlen(cmd));
		if (has_error == 1)
			write(2, ": Permission denied\n", 20);
		else
			write(2, ": Command not found\n", 20);
		exit(1);
	}
	return (child_pid);
}

pid_t	child_procces2(char *cmd, t_context *context)
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
		dup2(context->pipe_io[0], STDIN_FILENO);
		dup2(context->io[1], STDOUT_FILENO);
		close(context->io[0]);
		close(context->io[1]);
		close(context->pipe_io[0]);
		close(context->pipe_io[1]);
		has_error = external_cmd(cmd);
		while (context->env[++i] != NULL)
		{
			if (ft_strncmp("PATH=", context->env[i], 5) == 0)
				use_cmd(cmd, context->env[i] + 5);
		}
		write(2, cmd, ft_strlen(cmd));
		if (has_error == 1)
			write(2, ": Permission denied\n", 20);
		else
			write(2, ": Command not found\n", 20);
		exit(1);
	}
	return (child_pid);	
}

int	main(int argc, char **argv, char **env)
{
	pid_t		child_pid;
	pid_t		child_pid2;
	t_context	context;

	error_manager(argc, argv);
	context.io[0] = open(argv[1], O_RDONLY);
	context.io[1] = open(argv[4], O_RDWR | O_TRUNC | O_CREAT, 0644);
	pipe(context.pipe_io);
	context.env = env;
	child_pid = child_procces(argv[2], &context);
	waitpid(child_pid, NULL, 0);
	child_pid2 = child_procces2(argv[3], &context);
	close(context.io[0]);
	close(context.io[1]);
	close(context.pipe_io[0]);
	close(context.pipe_io[1]);
	waitpid(child_pid2, NULL, 0);
	return (0);
}
