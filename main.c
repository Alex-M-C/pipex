#include "pipex.h"

void	use_cmd(int argc, char **argv, char *paths)
{
	char	**path;
	char	**args;
	int		i;
	int		j;

	path = ft_split(paths, ':');
	if (!path)
	{
		perror("Error:");
		exit(1);
	}
	i = -1;
	j = 2;
	while (path[++i] != NULL)
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], argv[j]);
		if (!path[i])
		{
			perror("Error:");
			//liberar path
			exit(1);
		}
		if (access(path[i], X_OK) == 0)
		{
			args = ft_split(argv[j++], ' ');
			if (!path)
			{
				perror("Error:");
				//liberar path
				exit(1);
			}
			execve(path[i], args, NULL);//Acaba aqui
		}
	}
	printf("Comando no encontrado");
	//liberar path, args
	exit(1);
}

void	error_manager(int argc, char **argv, char **env)
{
	//int	i;

	if (argc != 5)
	{
		printf("Cantidad de argumentos incorrecta\n");
		exit(1);
	}
	if (access(argv[1], F_OK | R_OK) == -1)
	{
		perror(argv[1]);
		exit(1);
	}
	if (access(argv[argc - 1], W_OK) == -1)
	{
		perror(argv[argc - 1]);
		exit(1);
	}
}

pid_t	child_procces(int argc, char **argv, t_context *context)
{
	pid_t		child_pid;
	int			i;

	i = -1;
	child_pid = fork();
	if (child_pid == -1)
	{
		return (-1);
	}
	else if (child_pid == 0)
	{
		dup2(context->io[0], 0);
		while (env[++i] != NULL)
		{
			if (ft_strncmp("PATH=", context->env[i], 5) == 0)
				use_cmd(argc, argv, context->env[i] + 5);
		}
	}
	else
	{
		wait(NULL);
	}
}

int	main(int argc, char **argv, char **env)
{
	int			fd[2];
	int			pipe_fd[2];
	pid_t		child_pid;
	t_context	*context;

	error_manager(argc, argv);
	fd[0] = open(argv[1], O_RDONLY);
	fd[1] = open(argv[4], O_CREAT | O_WRONLY, 666);
	pipe(pipe_fd);
	context->env = env;
	context->io = fd;
	context->pipe_io = pipe_fd;
	child_procces(argc, argv, context);
	retrun(0);
}
