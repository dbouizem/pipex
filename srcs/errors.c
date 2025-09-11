#include "../includes/pipex.h"

void	error_exit(char *msg, int exit_code)
{
	if (msg)
	{
		write(2, "pipex: ", 7);
		write(2, msg, ft_strlen(msg));
		write(2, "\n", 1);
	}
	exit(exit_code);
}

void	handle_fork_error(t_pipex *pipex, pid_t pid1)
{
	if (pid1 > 0)
	{
		waitpid(pid1, NULL, 0);
	}
	free_resources(pipex);
	error_exit("fork failed", 1);
}

void	handle_file_error(char *filename)
{
	write(2, "pipex: ", 7);
	if (filename)
		write(2, filename, ft_strlen(filename));
	else
		write(2, "(null)", 6);
	write(2, ": ", 2);
	if (errno == EACCES)
		write(2, "Permission denied\n", 18);
	else if (errno == ENOENT)
		write(2, "No such file or directory\n", 26);
	else if (errno == EISDIR)
		write(2, "Is a directory\n", 15);
	else
		write(2, "File error\n", 11);
}
