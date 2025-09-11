#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include "../libft/libft.h"

# define READ_END 0
# define WRITE_END 1

typedef struct s_pipex_bonus
{
	int		infile;
	int		outfile;
	int		**pipes;
	pid_t	*pids;
	char	***cmd_args;
	char	**cmd_paths;
	int		cmd_count;
	int		pipe_count;
	int		here_doc;
	char	*limiter;
}	t_pipex_bonus;

// Fonctions d'initialisation
void	init_pipex_bonus(t_pipex_bonus *pipex);
void	open_files_bonus(t_pipex_bonus *pipex, char **argv, int argc);
void	prepare_commands_bonus(t_pipex_bonus *pipex, char **argv, char **envp);

// Gestion des processus multiples
void	create_processes_bonus(t_pipex_bonus *pipex, char **envp);
void	child_process_bonus(t_pipex_bonus *pipex, char **envp, int cmd_index);
void	setup_child_io_bonus(t_pipex_bonus *pipex, int cmd_index);

// Gestion des pipes multiples
void	create_pipes_bonus(t_pipex_bonus *pipex);
void	close_all_pipes_bonus(t_pipex_bonus *pipex);

// Here_doc
void	handle_here_doc(t_pipex_bonus *pipex);
int		is_here_doc(char **argv);

// Utilitaires
char	*find_path_bonus(char *cmd, char **envp);
void	free_array_bonus(char **arr);

// Gestion d'erreurs
void	error_exit_bonus(char *msg, int exit_code);
void	handle_command_error_bonus(char **cmd_args, int cmd_index);
void	handle_file_error_bonus(char *filename);
void	handle_command_not_found(t_pipex_bonus *pipex, int cmd_index);

// Nettoyage
void	free_resources_bonus(t_pipex_bonus *pipex);

#endif
