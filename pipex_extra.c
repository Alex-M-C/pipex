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

#include "pipex.h"

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
