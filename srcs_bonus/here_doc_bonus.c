#include "../includes/pipex_bonus.h"

int	is_here_doc(char **argv)
{
	if (argv[1] && ft_strncmp(argv[1], "here_doc", 8) == 0
		&& (!argv[1][8] || argv[1][8] == '\0'))
		return (1);
	return (0);
}

static char	*read_line_from_stdin(void)
{
	char	*line;
	char	buffer;
	int		i;
	int		size;

	size = 128;
	line = malloc(size);
	if (!line)
		return (NULL);
	i = 0;
	while (read(0, &buffer, 1) > 0)
	{
		if (buffer == '\n')
			break ;
		if (i >= size - 1)
		{
			size *= 2;
			line = ft_realloc_double(line, size);
			if (!line)
				return (NULL);
		}
		line[i++] = buffer;
	}
	line[i] = '\0';
	return (line);
}

static void	write_line_to_pipe(int pipe_fd, char *line)
{
	write(pipe_fd, line, ft_strlen(line));
	write(pipe_fd, "\n", 1);
}

void	handle_here_doc(t_pipex_bonus *pipex)
{
	int		pipefd[2];
	char	*line;
	int		limiter_len;

	if (pipe(pipefd) < 0)
		error_exit_bonus("pipe failed", 1);
	limiter_len = ft_strlen(pipex->limiter);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = read_line_from_stdin();
		if (!line)
			break ;
		if (ft_strncmp(line, pipex->limiter, limiter_len) == 0
			&& line[limiter_len] == '\0')
		{
			free(line);
			break ;
		}
		write_line_to_pipe(pipefd[1], line);
		free(line);
	}
	close(pipefd[1]);
	pipex->infile = pipefd[0];
}
