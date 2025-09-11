#include "../includes/pipex_bonus.h"

static void	setup_first_child_io(t_pipex_bonus *pipex)
{
	if (pipex->infile < 0)
	{
		pipex->infile = open("/dev/null", O_RDONLY);
		if (pipex->infile < 0)
			exit(1);
	}
	if (dup2(pipex->infile, STDIN_FILENO) < 0)
		exit(1);
	if (pipex->cmd_count == 1)
	{
		if (dup2(pipex->outfile, STDOUT_FILENO) < 0)
			exit(1);
	}
	else
	{
		if (dup2(pipex->pipes[0][WRITE_END], STDOUT_FILENO) < 0)
			exit(1);
	}
}

static void	setup_last_child_io(t_pipex_bonus *pipex, int cmd_index)
{
	if (dup2(pipex->pipes[cmd_index - 1][READ_END], STDIN_FILENO) < 0)
		exit(1);
	if (dup2(pipex->outfile, STDOUT_FILENO) < 0)
		exit(1);
}

static void	setup_middle_child_io(t_pipex_bonus *pipex, int cmd_index)
{
	if (dup2(pipex->pipes[cmd_index - 1][READ_END], STDIN_FILENO) < 0)
		exit(1);
	if (dup2(pipex->pipes[cmd_index][WRITE_END], STDOUT_FILENO) < 0)
		exit(1);
}

void	setup_child_io_bonus(t_pipex_bonus *pipex, int cmd_index)
{
	if (cmd_index == 0)
		setup_first_child_io(pipex);
	else if (cmd_index == pipex->cmd_count - 1)
		setup_last_child_io(pipex, cmd_index);
	else
		setup_middle_child_io(pipex, cmd_index);
}

void	child_process_bonus(t_pipex_bonus *pipex, char **envp, int cmd_index)
{
	setup_child_io_bonus(pipex, cmd_index);
	close_all_pipes_bonus(pipex);
	if (!pipex->cmd_paths[cmd_index])
		handle_command_not_found(pipex, cmd_index);
	if (!pipex->cmd_args[cmd_index] || !pipex->cmd_args[cmd_index][0])
	{
		free_resources_bonus(pipex);
		exit(127);
	}
	execve(pipex->cmd_paths[cmd_index], pipex->cmd_args[cmd_index], envp);
	free_resources_bonus(pipex);
	if (errno == EACCES)
		exit(126);
	exit(127);
}
