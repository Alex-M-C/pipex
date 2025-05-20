/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:05:39 by aleconst          #+#    #+#             */
/*   Updated: 2025/05/20 12:05:44 by aleconst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# define ERROR_MALLOC "Error: Memory full"

typedef struct s_context
{
	int		argc;
	char	**env;
	int		io[2];
	int		order;
	char	*out_name;
	int		pipe_io[2];
	int		pipe2_io[2];//bonus
}	t_context;

void	stderror_manager(char *message, int has_prerror, int exit_mode,
			t_context context);

void	create_path(char **s1, char *s2);

void	redirect_io(t_context *context);

#endif
