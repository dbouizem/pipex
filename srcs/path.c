#include "../includes/pipex.h"

static char	*find_env_path(char **envp)
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

static char	*search_in_paths(char **paths, char *slash_cmd)
{
	char	*path;
	int		i;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], slash_cmd);
		if (path && access(path, X_OK) == 0)
		{
			return (path);
		}
		if (path)
			free(path);
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*slash_cmd;
	char	*env_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	env_path = find_env_path(envp);
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	slash_cmd = ft_strjoin("/", cmd);
	if (!slash_cmd)
		return (free_array(paths), NULL);
	path = search_in_paths(paths, slash_cmd);
	return (free(slash_cmd), free_array(paths), path);
}
