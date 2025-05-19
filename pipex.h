#ifndef PIPEX_H
# define PIPEX_H
# include <fcntl.h>
# include <unistd.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <wait.h>
# include "libft/libft.h"

typedef struct s_context
{
    char    **env;
    int     io[2];
    int     pipe_io[2];
    int     pipe2_io[2];//bonus
    int     order;//bonus
}   t_context;

void	clear_context(t_context *context);

#endif