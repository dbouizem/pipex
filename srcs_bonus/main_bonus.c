#include "../includes/pipex_bonus.h"

static int	validate_arguments_bonus(int argc, char **argv)
{
	if (is_here_doc(argv))
	{
		if (argc < 6)
		{
			write(2, "./pipex_bonus here_doc LIMITER cmd1 cmd2 file\n", 47);
			return (0);
		}
	}
	else
	{
		if (argc < 5)
		{
			write(2, "Usage: ./pipex_bonus file1 cmd1 [cmd2...] file2\n", 49);
			return (0);
		}
	}
	return (1);
}

static void	init_pipex_structure(t_pipex_bonus *pipex, int argc, char **argv)
{
	if (is_here_doc(argv))
	{
		pipex->here_doc = 1;
		pipex->limiter = argv[2];
		pipex->cmd_count = argc - 4;
	}
	else
	{
		pipex->here_doc = 0;
		pipex->limiter = NULL;
		pipex->cmd_count = argc - 3;
	}
	pipex->pipe_count = pipex->cmd_count - 1;
}

static int	wait_for_all_children(t_pipex_bonus *pipex)
{
	int	i;
	int	status;
	int	exit_code;

	exit_code = 0;
	i = 0;
	while (i < pipex->cmd_count)
	{
		waitpid(pipex->pids[i], &status, 0);
		if (i == pipex->cmd_count - 1)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
			else
				exit_code = 1;
		}
		i++;
	}
	return (exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex_bonus	pipex;
	int				exit_code;

	if (!validate_arguments_bonus(argc, argv))
		exit(1);
	if (!envp)
	{
		write(2, "Error: no environment\n", 22);
		exit(1);
	}
	init_pipex_structure(&pipex, argc, argv);
	init_pipex_bonus(&pipex);
	open_files_bonus(&pipex, argv, argc);
	prepare_commands_bonus(&pipex, argv, envp);
	create_processes_bonus(&pipex, envp);
	close_all_pipes_bonus(&pipex);
	exit_code = wait_for_all_children(&pipex);
	free_resources_bonus(&pipex);
	exit(exit_code);
}
