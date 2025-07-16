#include "minishell.h"

// Sale limpiamente de la minishell cerrando todos los descriptores abiertos y liberando la memoria.
// Si la estructura de datos es válida, libera los recursos asociados:
//   - Cierra los descriptores de archivos de los comandos si existen.
//   - Libera toda la memoria dinámica utilizada por la shell.
// Finalmente, termina el proceso con el código de salida proporcionado.
void exit_shell(t_data *data, int exno)
{
	if (data)
	{
		if (data->cmd && data->cmd->io_fds)
			close_fds(data->cmd, true); // Cierra los fds y restaura los backups
		free_data(data, true); // Libera toda la memoria y limpia el historial
	}
	exit(exno); // Termina el proceso con el código de salida 