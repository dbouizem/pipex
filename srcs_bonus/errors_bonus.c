#include "../includes/pipex_bonus.h"

void	error_exit_bonus(char *msg, int exit_code)
{
	if (msg)
	{
		write(2, "pipex: ", 7);
		write(2, msg, ft_strlen(msg));
		write(2, "\n", 1);
	}
	exit(exit_code);
}

void	handle_command_error_bonus(char **cmd_args, int cmd_index)
{
	(void)cmd_index;
	write(2, "pipex: ", 7);
	if (cmd_args && cmd_args[0])
		write(2, cmd_args[0], ft_strlen(cmd_args[0]));
	else
		write(2, "(null)", 6);
	write(2, ": command not found\n", 20);
	exit(127);
}

void	handle_file_error_bonus(char *filename)
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

void	handle_command_not_found(t_pipex_bonus *pipex, int cmd_index)
{
	size_t	len;

	free_resources_bonus(pipex);
	write(2, "pipex: ", 7);
	if (pipex->cmd_args && pipex->cmd_args[cmd_index]
		&& pipex->cmd_args[cmd_index][0])
	{
		len = ft_strlen(pipex->cmd_args[cmd_index][0]);
		write(2, pipex->cmd_args[cmd_index][0], len);
	}
	else
		write(2, "(null)", 6);
	write(2, ": command not found\n", 20);
	exit(127);
}
