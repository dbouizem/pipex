#include "../includes/pipex_bonus.h"

static void	kill_created_processes(t_pipex_bonus *pipex, int created_count)
{
	int	i;

	i = 0;
	while (i < created_count)
	{
		if (pipex->pids[i] > 0)
			waitpid(pipex->pids[i], NULL, 0);
		i++;
	}
}

static void	handle_fork_error_bonus(t_pipex_bonus *pipex, int created_count)
{
	kill_created_processes(pipex, created_count);
	free_resources_bonus(pipex);
	error_exit_bonus("fork failed", 1);
}

static void	fork_single_process(t_pipex_bonus *pipex, char **envp, int i)
{
	pipex->pids[i] = fork();
	if (pipex->pids[i] < 0)
		handle_fork_error_bonus(pipex, i);
	if (pipex->pids[i] == 0)
		child_process_bonus(pipex, envp, i);
}

void	create_processes_bonus(t_pipex_bonus *pipex, char **envp)
{
	int	i;

	create_pipes_bonus(pipex);
	i = 0;
	while (i < pipex->cmd_count)
	{
		fork_single_process(pipex, envp, i);
		i++;
	}
}
