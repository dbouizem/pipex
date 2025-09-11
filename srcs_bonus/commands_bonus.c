#include "../includes/pipex_bonus.h"

static int	validate_command_bonus(char *cmd_str)
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

static void	parse_single_command(t_pipex_bonus *pipex, char *cmd_str, int index)
{
	pipex->cmd_args[index] = ft_split(cmd_str, ' ');
	if (!pipex->cmd_args[index])
	{
		free_resources_bonus(pipex);
		error_exit_bonus("malloc failed", 1);
	}
	if (!pipex->cmd_args[index][0])
	{
		free_resources_bonus(pipex);
		error_exit_bonus("empty command after parsing", 127);
	}
}

static int	get_cmd_start_index(t_pipex_bonus *pipex)
{
	if (pipex->here_doc)
		return (3);
	else
		return (2);
}

void	prepare_commands_bonus(t_pipex_bonus *pipex, char **argv, char **envp)
{
	int	i;
	int	cmd_arg_start;

	cmd_arg_start = get_cmd_start_index(pipex);
	i = 0;
	while (i < pipex->cmd_count)
	{
		if (!validate_command_bonus(argv[cmd_arg_start + i]))
		{
			write(2, "pipex: empty command\n", 21);
			free_resources_bonus(pipex);
			exit(127);
		}
		parse_single_command(pipex, argv[cmd_arg_start + i], i);
		pipex->cmd_paths[i] = find_path_bonus(pipex->cmd_args[i][0], envp);
		i++;
	}
}
