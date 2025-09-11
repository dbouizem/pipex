#include "../includes/pipex_bonus.h"

void	create_pipes_bonus(t_pipex_bonus *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->pipe_count)
	{
		if (pipe(pipex->pipes[i]) < 0)
		{
			free_resources_bonus(pipex);
			error_exit_bonus("pipe failed", 1);
		}
		i++;
	}
}

static void	close_pipe_fds(t_pipex_bonus *pipex, int pipe_index)
{
	if (pipex->pipes && pipex->pipes[pipe_index])
	{
		if (pipex->pipes[pipe_index][READ_END] >= 0)
			close(pipex->pipes[pipe_index][READ_END]);
		if (pipex->pipes[pipe_index][WRITE_END] >= 0)
			close(pipex->pipes[pipe_index][WRITE_END]);
	}
}

void	close_all_pipes_bonus(t_pipex_bonus *pipex)
{
	int	i;

	if (!pipex)
		return ;
	if (pipex->infile >= 0)
	{
		close(pipex->infile);
		pipex->infile = -1;
	}
	if (pipex->outfile >= 0)
	{
		close(pipex->outfile);
		pipex->outfile = -1;
	}
	if (pipex->pipes && pipex->pipe_count > 0)
	{
		i = 0;
		while (i < pipex->pipe_count)
		{
			close_pipe_fds(pipex, i);
			i++;
		}
	}
}
