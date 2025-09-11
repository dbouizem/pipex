#include "../includes/pipex_bonus.h"

void	free_array_bonus(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	*search_in_paths_bonus(char **paths, char *slash_cmd)
{
	char	*path;
	int		i;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], slash_cmd);
		if (path && access(path, X_OK) == 0)
			return (path);
		if (path)
			free(path);
		i++;
	}
	return (NULL);
}

static char	*find_env_path_bonus(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*handle_absolute_path(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*find_path_bonus(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*slash_cmd;
	char	*env_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (handle_absolute_path(cmd));
	env_path = find_env_path_bonus(envp);
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	slash_cmd = ft_strjoin("/", cmd);
	if (!slash_cmd)
		return (free_array_bonus(paths), NULL);
	path = search_in_paths_bonus(paths, slash_cmd);
	return (free(slash_cmd), free_array_bonus(paths), path);
}
