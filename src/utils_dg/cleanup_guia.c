#include "minishell.h"

// Libera la memoria asociada a la estructura principal de datos de la shell.
// Si clear_history es true, también libera el historial de readline y los directorios de trabajo.
void free_data(t_data *data, bool clear_history)
{
	if (data && data->user_input)
	{
		free_ptr(data->user_input); // Libera la entrada del usuario
		data->user_input = NULL;
	}
	if (data && data->token)
		lstclear_token(&data->token, &free_ptr); // Libera la lista de tokens
	if (data && data->cmd)
		lst_clear_cmd(&data->cmd, &free_ptr); // Libera la lista de comandos
	if (clear_history == true)
	{
		if (data && data->working_dir)
			free_ptr(data->working_dir); // Libera el directorio de trabajo actual
		if (data && data->old_working_dir)
			free_ptr(data->old_working_dir); // Libera el directorio de trabajo anterior
		if (data && data->env)
			free_str_tab(data->env); // Libera el array de variables de entorno
		rl_clear_history(); // Limpia el historial de readline
	}
}

// Cierra los descriptores de archivos asociados a los comandos.
// Si close_backups es true, también restaura los backups de stdin/stdout.
void close_fds(t_command *cmds, bool close_backups)
{
	if (cmds->io_fds)
	{
		if (cmds->io_fds->fd_in != -1)
			close(cmds->io_fds->fd_in); // Cierra el descriptor de entrada
		if (cmds->io_fds->fd_out != -1)
			close(cmds->io_fds->fd_out); // Cierra el descriptor de salida
		if (close_backups)
			restore_io(cmds->io_fds); // Restaura los backups de stdin/stdout
	}
	close_pipe_fds(cmds, NULL); // Cierra los pipes asociados
}

// Libera la estructura de ficheros de entrada/salida de un comando.
void free_io(t_io_fds *io)
{
	if (!io)
		return;
	restore_io(io); // Restaura los descriptores originales
	if (io->heredoc_delimiter)
	{
		unlink(io->infile); // Elimina el archivo temporal de heredoc
		free_ptr(io->heredoc_delimiter); // Libera el delimitador
	}
	if (io->infile)
		free_ptr(io->infile); // Libera el nombre del archivo de entrada
	if (io->outfile)
		free_ptr(io->outfile); // Libera el nombre del archivo de salida
	if (io)
		free_ptr(io); // Libera la estructura en sí
}

// Libera un array de strings (como el de variables de entorno).
void free_str_tab(char **tab)
{
	int i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			if (tab[i])
			{
				free_ptr(tab[i]); // Libera cada string
				tab[i] = NULL;
			}
			i++;
		}
		free(tab); // Libera el array de punteros
		tab = NULL;
	}
}

// Libera un puntero de cualquier tipo si no es NULL y lo pone a NULL.
// Evita dobles liberaciones accidentales.
void free_ptr(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
} 