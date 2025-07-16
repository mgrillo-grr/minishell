#include "minishell.h"

// Variable global para almacenar el último código de salida de la shell
int g_last_exit_code;

// Espera a que terminen todos los procesos hijos y devuelve el código de salida adecuado.
// Si un hijo termina por señal, devuelve 128 + número de señal.
// Si termina normalmente, devuelve su código de salida.
static int get_children(t_data *data)
{
	pid_t wpid;
	int status;
	int save_status;

	close_fds(data->cmd, false); // Cierra los descriptores de los comandos
	save_status = 0;
	wpid = 0;
	while (wpid != -1 || errno != ECHILD)
	{
		wpid = waitpid(-1, &status, 0); // Espera cualquier hijo
		if (wpid == data->pid)
			save_status = status; // Guarda el status del último hijo relevante
		continue;
	}
	if (WIFSIGNALED(save_status))
		status = 128 + WTERMSIG(save_status); // Terminó por señal
	else if (WIFEXITED(save_status))
		status = WEXITSTATUS(save_status);   // Terminó normalmente
	else
		status = save_status;
	return (status);
}

// Crea un proceso hijo por cada comando a ejecutar (excepto builtins no pipelined).
// Devuelve el código de salida del último hijo.
static int create_children(t_data *data)
{
	t_command *cmd;

	cmd = data->cmd;
	while (data->pid != 0 && cmd)
	{
		data->pid = fork();
		if (data->pid == -1)
			return (errmsg_cmd(MSG_FORK, NULL, strerror(errno), EXIT_FAILURE));
		else if (data->pid == 0)
			execute_command(data, cmd); // El hijo ejecuta el comando
		cmd = cmd->next;
	}
	return (get_children(data));
}

// Prepara la lista de comandos para la ejecución: crea pipes y verifica archivos.
// Devuelve CMD_NOT_FOUND si todo está listo, EXIT_FAILURE si hay error, EXIT_SUCCESS si no hay comandos.
static int prep_for_exec(t_data *data)
{
	if (!data || !data->cmd)
		return (EXIT_SUCCESS);
	if (!data->cmd->command)
	{
		if (data->cmd->io_fds && !check_infile_outfile(data->cmd->io_fds))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	if (!create_pipes(data))
		return (EXIT_FAILURE);
	return (CMD_NOT_FOUND);
}

// Función principal para ejecutar los comandos de la shell.
// Ejecuta builtins directamente si no hay pipes, o crea hijos para los demás casos.
// Devuelve el código de salida del último comando ejecutado.
int execute(t_data *data)
{
	int ret;

	ret = prep_for_exec(data);
	if (ret != CMD_NOT_FOUND)
		return (ret);
	if (!data->cmd->pipe_output && !data->cmd->prev && check_infile_outfile(data->cmd->io_fds))
	{
		redirect_io(data->cmd->io_fds); // Redirige IO si es necesario
		ret = execute_builtin(data, data->cmd); // Ejecuta el builtin
		restore_io(data->cmd->io_fds); // Restaura IO
	}
	if (ret != CMD_NOT_FOUND)
		return (ret);
	return (create_children(data)); // Ejecuta el resto de comandos en hijos
} 