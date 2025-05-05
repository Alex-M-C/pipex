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
}   t_context;

#endif