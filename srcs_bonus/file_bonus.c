#include "../includes/pipex_bonus.h"

static void	open_infile_normal(t_pipex_bonus *pipex, char **argv)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
		handle_file_error_bonus(argv[1]);
}

static void	open_outfile(t_pipex_bonus *pipex, char *filename, int mode)
{
	if (mode == 1)
		pipex->outfile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		pipex->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile < 0)
	{
		handle_file_error_bonus(filename);
		free_resources_bonus(pipex);
		exit(1);
	}
}

void	open_files_bonus(t_pipex_bonus *pipex, char **argv, int argc)
{
	int	last_arg_index;

	last_arg_index = argc - 1;
	if (pipex->here_doc)
	{
		handle_here_doc(pipex);
		open_outfile(pipex, argv[last_arg_index], 1);
	}
	else
	{
		open_infile_normal(pipex, argv);
		open_outfile(pipex, argv[last_arg_index], 0);
	}
}
