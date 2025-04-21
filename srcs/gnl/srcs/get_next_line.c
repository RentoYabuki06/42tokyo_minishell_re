/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 19:05:04 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/21 14:02:04 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/get_next_line.h"

static char	*read_to_newline(int fd, char **saved, char *buffer)
{
	char	*join_buf;
	ssize_t	bytes_read;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1 && errno == EINTR)
			return (free(*saved), NULL);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		join_buf = ft_strjoin(*saved, buffer);
		free(*saved);
		if (join_buf == NULL)
			return (NULL);
		*saved = join_buf;
		if (ft_strchr(*saved, '\n'))
			break ;
	}
	if ((bytes_read == 0 && ft_strlen(*saved) == 0) || bytes_read < 0)
		return (free(*saved), write(STDOUT_FILENO, "\n", 1), NULL);
	return (*saved);
}

static char	*extract_line(char *saved, size_t len)
{
	size_t	i;
	char	*line;

	i = 0;
	line = (char *)malloc((len + 2) * sizeof(char));
	if (line == NULL)
		return (free(saved), NULL);
	while (i <= len)
	{
		line[i] = saved[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*ft_shift_pointer(size_t len, char *saved)
{
	char	*tmp;

	tmp = ft_strdup(saved + len + 1);
	free(saved);
	saved = NULL;
	if (tmp == NULL)
		return (NULL);
	return (tmp);
}

char	*get_next_line(int fd, char **saved)
{
	char		*line;
	char		*buffer;
	size_t		len;

	if (fd < 0 || BUFFER_SIZE <= 0 || saved == NULL)
		return (NULL);
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buffer == NULL)
		return (NULL);
	*saved = read_to_newline(fd, saved, buffer);
	free(buffer);
	if (*saved == NULL)
		return (NULL);
	len = 0;
	while ((*saved)[len + 1] != '\0' && (*saved)[len] != '\n')
		len++;
	line = extract_line(*saved, len);
	if (line == NULL)
		return (NULL);
	*saved = ft_shift_pointer(len, *saved);
	if (*saved == NULL)
		return (free(line), NULL);
	return (line);
}
