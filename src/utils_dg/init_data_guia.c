#include "minishell.h"

// Inicializa la variable de entorno en la estructura de datos
static bool init_env(t_data *data, char **env)
{
	int i; // Índice para recorrer el array de variables de entorno

	// Reserva memoria para el array de punteros a strings que contendrá las variables de entorno
	// Se suma 1 para el puntero NULL final que marca el fin del array
	data->env = ft_calloc(env_var_count(env) + 1, sizeof * data->env);
	if (!data->env)
		return (false); // Si falla la reserva de memoria, retorna error
	i = 0;
	while (env[i]) // Recorre todas las variables de entorno originales
	{
		// Duplica cada string de variable de entorno y lo guarda en el nuevo array
		data->env[i] = ft_strdup(env[i]);
		if (!data->env[i])
			return (false); // Si falla la duplicación, retorna error
		i++;
	}
	// El último puntero queda en NULL (por calloc), marcando el fin del array
	return (true); // Éxito
}

// Inicializa los directorios de trabajo actuales y anteriores
static bool init_wds(t_data *data)
{
	char buff[PATH_MAX]; // Buffer para almacenar el path actual
	char *wd; // Puntero al path actual

	// Obtiene el directorio de trabajo actual y lo guarda en buff
	wd = getcwd(buff, PATH_MAX);
	// Duplica el path actual y lo guarda en la estructura
	data->working_dir = ft_strdup(wd);
	if (!data->working_dir)
		return (false); // Si falla la duplicación, retorna error
	// Verifica si OLDPWD existe en el entorno
	if (get_env_var_index(data->env, "OLDPWD") != -1)
	{
		// Si existe OLDPWD, lo copia como old_working_dir
		data->old_working_dir = ft_strdup(get_env_var_value(data->env, "OLDPWD"));
		if (!data->old_working_dir)
			return (false); // Si falla la duplicación, retorna error
	}
	else
	{
		// Si no existe OLDPWD, usa el directorio actual como old_working_dir
		data->old_working_dir = ft_strdup(wd);
		if (!data->old_working_dir)
			return (false);
	}
	return (true);
}

// Inicializa la estructura principal de datos para la shell
bool init_data(t_data *data, char **env)
{
	// Inicializa las variables de entorno copiando el entorno original
	if (!init_env(data, env))
	{
		// Si falla, muestra mensaje de error y retorna false
		errmsg_cmd(MSG_FATAL, NULL, E_INIT_ENV, 1);
		return (false);
	}
	// Inicializa los directorios de trabajo (PWD y OLDPWD)
	if (!init_wds(data))
	{
		// Si falla, muestra mensaje de error y retorna false
		errmsg_cmd(MSG_FATAL, NULL, E_INIT_WDS, 1);
		return (false);
	}
	// Inicializa los campos restantes de la estructura de datos
	data->token = NULL; // Lista de tokens (aún vacía)
	data->user_input = NULL; // Comando del usuario (aún vacío)
	data->cmd = NULL; // Lista de comandos (aún vacía)
	data->pid = -1; // PID inicializado en -1 (sin proceso hijo)
	g_last_exit_code = 0; // Código de salida global en 0 (éxito)
	return (true); // Éxito
}

// Inicializa la estructura de ficheros de entrada/salida para un comando
void init_io(t_command *cmd)
{
	// Si la estructura de ficheros no existe, la crea
	if (!cmd->io_fds)
	{
		// Reserva memoria para la estructura de ficheros
		cmd->io_fds = malloc(sizeof * cmd->io_fds);
		if (!cmd->io_fds)
			return ; // Si falla la reserva, sale
		// Inicializa todos los campos de la estructura en valores por defecto
		cmd->io_fds->infile = NULL; // Archivo de entrada (ninguno)
		cmd->io_fds->outfile = NULL; // Archivo de salida (ninguno)
		cmd->io_fds->heredoc_delimiter = NULL; // Delimitador de heredoc (ninguno)
		cmd->io_fds->heredoc_quotes = false; // Heredoc sin comillas por defecto
		cmd->io_fds->fd_in = -1; // Descriptor de entrada inicializado en -1
		cmd->io_fds->fd_out = -1; // Descriptor de salida inicializado en -1
		cmd->io_fds->stdin_backup = -1; // Backup de stdin inicializado en -1
		cmd->io_fds->stdout_backup = -1; // Backup de stdout inicializado en -1
	}
} 