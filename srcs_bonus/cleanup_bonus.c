#include "../includes/pipex_bonus.h"

static void	free_commands_bonus(t_pipex_bonus *pipex)
{
	int	i;

	if (!pipex->cmd_args)
		return ;
	i = 0;
	while (i < pipex->cmd_count)
	{
		if (pipex->cmd_args[i])
			free_array_bonus(pipex->cmd_args[i]);
		i++;
	}
	free(pipex->cmd_args);
	pipex->cmd_args = NULL;
}

static void	free_paths_bonus(t_pipex_bonus *pipex)
{
	int	i;

	if (!pipex->cmd_paths)
		return ;
	i = 0;
	while (i < pipex->cmd_count)
	{
		if (pipex->cmd_paths[i])
			free(pipex->cmd_paths[i]);
		i++;
	}
	free(pipex->cmd_paths);
	pipex->cmd_paths = NULL;
}

static void	free_pipes_bonus(t_pipex_bonus *pipex)
{
	int	i;

	if (!pipex->pipes)
		return ;
	i = 0;
	while (i < pipex->pipe_count)
	{
		if (pipex->pipes[i])
			free(pipex->pipes[i]);
		i++;
	}
	free(pipex->pipes);
	pipex->pipes = NULL;
}

void	free_resources_bonus(t_pipex_bonus *pipex)
{
	if (!pipex)
		return ;
	free_commands_bonus(pipex);
	free_paths_bonus(pipex);
	free_pipes_bonus(pipex);
	if (pipex->pids)
	{
		free(pipex->pids);
		pipex->pids = NULL;
	}
}
