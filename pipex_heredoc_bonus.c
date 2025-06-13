/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 13:46:39 by aleconst          #+#    #+#             */
/*   Updated: 2025/06/13 13:46:41 by aleconst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	manage_line(char *delimiter, char *line, int n_line)
{
	if (line)
		free(line);
	else
	{
		write(2, "\nwarning: here-document at line ", 32);
		ft_putnbr_fd(n_line, 2);
		write(2, " delimited by end-of-file (wanted «", 36);
		write(2, delimiter, ft_strlen(delimiter));
		write(2, "»)\n", 4);
	}
}
