#include "minishell.h"

// Busca la ruta válida de un comando en los directorios del PATH.
// Devuelve la ruta completa si la encuentra, o NULL si no existe o no es ejecutable.
static char *find_valid_cmd_path(char *cmd, char **paths)
{
	int i;
	char *cmd_path;

	cmd_path = NULL;
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd); // Une el path con el comando
		if (!cmd_path)
		{
			errmsg_cmd(MSG_FATAL, NULL, E_UNEXPECTED_ERROR, EXIT_FAILURE);
			return (NULL);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path); // Encontró un ejecutable válido
		free_ptr(cmd_path);
		i++;
	}
	return (NULL);
}

// Obtiene los paths del entorno (PATH) y los separa en un array.
// Devuelve NULL si no existe PATH o hay error de memoria.
static char **get_paths_from_env(t_data *data)
{
	char **env_paths;

	if (get_env_var_index(data->env, "PATH") == -1)
		return (NULL);
	env_paths = ft_split(get_env_var_value(data->env, "PATH"), ':');
	if (!env_paths)
		return (NULL);
	return (env_paths);
}

// Devuelve la ruta completa del comando buscándolo en los directorios del PATH.
// Si no se encuentra, retorna NULL.
char *get_cmd_path(t_data *data, char *name)
{
	char **env_paths;
	char *cmd;
	char *cmd_path;

	if (!name)
		return (NULL);
	env_paths = get_paths_from_env(data);
	if (!env_paths)
		return (NULL);
	cmd = ft_strjoin("/", name); // Prepara el nombre del comando con barra
	if (!cmd)
	{
		free_str_tab(env_paths);
		return (NULL);
	}
	cmd_path = find_valid_cmd_path(cmd, env_paths);
	if (!cmd_path)
	{
		free_ptr(cmd);
		free_str_tab(env_paths);
		return (NULL);
	}
	return (cmd_path);
} 