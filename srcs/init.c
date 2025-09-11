#include "../includes/pipex.h"

void	init_pipex(t_pipex *pipex)
{
	pipex->infile = -1;
	pipex->outfile = -1;
	pipex->pipefd[0] = -1;
	pipex->pipefd[1] = -1;
	pipex->pid1 = -1;
	pipex->pid2 = -1;
	pipex->cmd1_args = NULL;
	pipex->cmd2_args = NULL;
	pipex->cmd1_path = NULL;
	pipex->cmd2_path = NULL;
}

void	open_files(t_pipex *pipex, char **argv)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
		handle_file_error(argv[1]);
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile < 0)
	{
		handle_file_error(argv[4]);
		free_resources(pipex);
		exit(1);
	}
}

static int	validate_command(char *cmd_str)
{
	if (!cmd_str || !*cmd_str)
		return (0);
	while (*cmd_str)
	{
		if (*cmd_str != ' ' && *cmd_str != '\t')
			return (1);
		cmd_str++;
	}
	return (0);
}

static void	parse_commands(t_pipex *pipex, char **argv)
{
	pipex->cmd1_args = ft_split(argv[2], ' ');
	pipex->cmd2_args = ft_split(argv[3], ' ');
	if (!pipex->cmd1_args || !pipex->cmd2_args)
	{
		free_resources(pipex);
		error_exit("malloc failed", 1);
	}
	if (!pipex->cmd1_args[0] || !pipex->cmd2_args[0])
	{
		free_resources(pipex);
		error_exit("empty command after parsing", 127);
	}
}

void	prepare_commands(t_pipex *pipex, char **argv, char **envp)
{
	int	err;

	err = 0;
	if (!validate_command(argv[2]))
	{
		write(2, "pipex: empty command 1\n", 23);
		err = 127;
	}
	if (!validate_command(argv[3]))
	{
		write(2, "pipex: empty command 2\n", 23);
		err = 127;
	}
	if (err)
	{
		free_resources(pipex);
		exit(127);
	}
	parse_commands(pipex, argv);
	pipex->cmd1_path = find_path(pipex->cmd1_args[0], envp);
	pipex->cmd2_path = find_path(pipex->cmd2_args[0], envp);
}
