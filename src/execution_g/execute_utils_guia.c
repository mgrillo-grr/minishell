#include "minishell.h"

// Verifica si el comando es un directorio usando stat.
// Devuelve true si es un directorio, false en caso contrario.
bool cmd_is_dir(char *cmd)
{
	struct stat cmd_stat;

	ft_memset(&cmd_stat, 0, sizeof(cmd_stat));
	stat(cmd, &cmd_stat);
	return (S_ISDIR(cmd_stat.st_mode));
}

// Verifica si el comando existe, es ejecutable y no es un directorio.
// Devuelve un código de error personalizado si no se puede ejecutar.
int check_command_not_found(t_data *data, t_command *cmd)
{
	if (ft_strchr(cmd->command, '/') == NULL && get_env_var_index(data->env, "PATH") != -1)
		return (errmsg_cmd(cmd->command, NULL, E_COMMAND_NOT_FOUND, CMD_NOT_FOUND));
	if (access(cmd->command, F_OK) != 0)
		return (errmsg_cmd(cmd->command, NULL, strerror(errno), CMD_NOT_FOUND));
	else if (cmd_is_dir(cmd->command))
		return (errmsg_cmd(cmd->command, NULL, E_IS_A_DIRECTORY, CMD_NOT_EXECUTABLE));
	else if (access(cmd->command, F_OK | X_OK) != 0)
		return (errmsg_cmd(cmd->command, NULL, strerror(errno), CMD_NOT_EXECUTABLE));
	return (EXIT_SUCCESS);
} 