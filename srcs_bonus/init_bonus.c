#include "../includes/pipex_bonus.h"

static void	init_pipes_array(t_pipex_bonus *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->pipe_count)
	{
		pipex->pipes[i] = malloc(sizeof(int) * 2);
		if (!pipex->pipes[i])
		{
			while (--i >= 0)
				free(pipex->pipes[i]);
			free(pipex->pipes);
			free(pipex->pids);
			free(pipex->cmd_args);
			free(pipex->cmd_paths);
			error_exit_bonus("malloc failed", 1);
		}
		pipex->pipes[i][0] = -1;
		pipex->pipes[i][1] = -1;
		i++;
	}
}

static void	init_arrays(t_pipex_bonus *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count)
	{
		pipex->pids[i] = -1;
		pipex->cmd_args[i] = NULL;
		pipex->cmd_paths[i] = NULL;
		i++;
	}
}

static void	allocate_arrays(t_pipex_bonus *pipex)
{
	pipex->pipes = malloc(sizeof(int *) * pipex->pipe_count);
	pipex->pids = malloc(sizeof(pid_t) * pipex->cmd_count);
	pipex->cmd_args = malloc(sizeof(char **) * pipex->cmd_count);
	pipex->cmd_paths = malloc(sizeof(char *) * pipex->cmd_count);
	if (!pipex->pipes || !pipex->pids || !pipex->cmd_args || !pipex->cmd_paths)
	{
		if (pipex->pipes)
			free(pipex->pipes);
		if (pipex->pids)
			free(pipex->pids);
		if (pipex->cmd_args)
			free(pipex->cmd_args);
		if (pipex->cmd_paths)
			free(pipex->cmd_paths);
		error_exit_bonus("malloc failed", 1);
	}
}

void	init_pipex_bonus(t_pipex_bonus *pipex)
{
	pipex->infile = -1;
	pipex->outfile = -1;
	allocate_arrays(pipex);
	if (pipex->pipe_count > 0)
		init_pipes_array(pipex);
	init_arrays(pipex);
}
