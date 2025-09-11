#include "../includes/pipex.h"

void	setup_child_io(t_pipex *pipex, int is_first)
{
	if (is_first)
	{
		if (pipex->infile < 0)
		{
			pipex->infile = open("/dev/null", O_RDONLY);
			if (pipex->infile < 0)
				exit(1);
		}
		if (dup2(pipex->infile, STDIN_FILENO) < 0)
			exit(1);
		if (dup2(pipex->pipefd[WRITE_END], STDOUT_FILENO) < 0)
			exit(1);
	}
	else
	{
		if (dup2(pipex->pipefd[READ_END], STDIN_FILENO) < 0)
			exit(1);
		if (dup2(pipex->outfile, STDOUT_FILENO) < 0)
			exit(1);
	}
}

static void	get_cmd(t_pipex *pipex, int is_first, char ***args, char **path)
{
	if (is_first)
	{
		*args = pipex->cmd1_args;
		*path = pipex->cmd1_path;
	}
	else
	{
		*args = pipex->cmd2_args;
		*path = pipex->cmd2_path;
	}
}

static void	command_not_found_exit(t_pipex *pipex, char **cmd_args)
{
	char	*cmd_name;

	cmd_name = NULL;
	if (cmd_args && cmd_args[0])
		cmd_name = ft_strdup(cmd_args[0]);
	free_resources(pipex);
	write(2, "pipex: ", 7);
	if (cmd_name)
	{
		write(2, cmd_name, ft_strlen(cmd_name));
		free(cmd_name);
	}
	else
		write(2, "(null)", 6);
	write(2, ": command not found\n", 20);
	exit(127);
}

void	create_processes(t_pipex *pipex, char **envp)
{
	pipex->pid1 = fork();
	if (pipex->pid1 < 0)
		handle_fork_error(pipex, 0);
	if (pipex->pid1 == 0)
		child_process(pipex, envp, 1);
	pipex->pid2 = fork();
	if (pipex->pid2 < 0)
		handle_fork_error(pipex, pipex->pid1);
	if (pipex->pid2 == 0)
		child_process(pipex, envp, 0);
}

void	child_process(t_pipex *pipex, char **envp, int is_first)
{
	char	**cmd_args;
	char	*cmd_path;

	setup_child_io(pipex, is_first);
	close_all(pipex);
	get_cmd(pipex, is_first, &cmd_args, &cmd_path);
	if (!cmd_path)
		command_not_found_exit(pipex, cmd_args);
	if (!cmd_args || !cmd_args[0])
	{
		free_resources(pipex);
		exit(127);
	}
	execve(cmd_path, cmd_args, envp);
	free_resources(pipex);
	if (errno == EACCES)
		exit(126);
	exit(127);
}
