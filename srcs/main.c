#include "../includes/pipex.h"

static void	wait_for_children(t_pipex *pipex)
{
	int	status1;
	int	status2;
	int	exit_code;

	close_all(pipex);
	waitpid(pipex->pid1, &status1, 0);
	waitpid(pipex->pid2, &status2, 0);
	if (WIFEXITED(status2))
		exit_code = WEXITSTATUS(status2);
	else if (WIFSIGNALED(status2))
		exit_code = 128 + WTERMSIG(status2);
	else
		exit_code = 1;
	free_resources(pipex);
	exit(exit_code);
}

static int	validate_arguments(int argc)
{
	if (argc != 5)
	{
		write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 38);
		return (0);
	}
	return (1);
}

static int	validate_environment(char **envp)
{
	if (!envp)
	{
		write(2, "Error: no environment\n", 22);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (!validate_arguments(argc))
		exit(1);
	if (!validate_environment(envp))
		exit(1);
	init_pipex(&pipex);
	open_files(&pipex, argv);
	prepare_commands(&pipex, argv, envp);
	if (pipe(pipex.pipefd) < 0)
	{
		free_resources(&pipex);
		error_exit("pipe failed", 1);
	}
	create_processes(&pipex, envp);
	wait_for_children(&pipex);
	return (0);
}
