#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include "../libft/libft.h"

# define READ_END 0
# define WRITE_END 1

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	char	**cmd1_args;
	char	**cmd2_args;
	char	*cmd1_path;
	char	*cmd2_path;
}	t_pipex;

// main
int		main(int argc, char **argv, char **envp);

// Fonctions init
void	init_pipex(t_pipex *pipex);
void	open_files(t_pipex *pipex, char **argv);
void	prepare_commands(t_pipex *pipex, char **argv, char **envp);

// Fonctions process
void	create_processes(t_pipex *pipex, char **envp);
void	child_process(t_pipex *pipex, char **envp, int is_first);
void	setup_child_io(t_pipex *pipex, int is_first);

// Fonctions utilitaires
char	*find_path(char *cmd, char **envp);

// Gestion d'erreurs
void	error_exit(char *msg, int exit_code);
void	handle_fork_error(t_pipex *pipex, pid_t pid1);
void	handle_file_error(char *filename);

// Nettoyage
void	free_array(char **arr);
void	close_all(t_pipex *pipex);
void	free_resources(t_pipex *pipex);

#endif
