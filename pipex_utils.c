#include "pipex.h"

void	clear_context(t_context *context)
{
	if (!context)
		return ;
	free(context);
}

void	stderror_manager(char *message)
{
	if (!message)
		return ;
	write(2, message, ft_strlen(message));
	exit (1);
}
