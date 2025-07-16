#include "minishell.h"

// Restaura los descriptores originales de entrada y salida estándar (stdin y stdout)
// usando los backups almacenados en la estructura io. Cierra los backups después de restaurar.
// Devuelve true si todo fue correcto, false si hubo algún error en dup2.
bool restore_io(t_io_fds *io)
{
	int ret;

	ret = true;
	if (!io)
		return (ret); // Si la estructura es nula, no hay nada que restaurar
	if (io->stdin_backup != -1)
	{
		if (dup2(io->stdin_backup, STDIN_FILENO) == -1)
			ret = false; // Error al restaurar stdin
		close(io->stdin_backup);
		io->stdin_backup = -1;
	}
	if (io->stdout_backup != -1)
	{
		if (dup2(io->stdout_backup, STDOUT_FILENO) == -1)
			ret = false; // Error al restaurar stdout
		close(io->stdout_backup);
		io->stdout_backup = -1;
	}
	return (ret);
}

// Redirige los descriptores de entrada y salida estándar a los archivos indicados en io.
// Realiza backups de los descriptores originales antes de redirigir.
// Devuelve true si todo fue correcto, o un valor de error si hubo problemas.
bool redirect_io(t_io_fds *io)
{
	int ret;

	ret = true;
	if (!io)
		return (ret); // Si la estructura es nula, no hay nada que redirigir
	io->stdin_backup = dup(STDIN_FILENO); // Backup de stdin
	if (io->stdin_backup == -1)
		ret = errmsg_cmd(MSG_DUP, MSG_STDIN_BACKUP, strerror(errno), false);
	io->stdout_backup = dup(STDOUT_FILENO); // Backup de stdout
	if (io->stdout_backup == -1)
		ret = errmsg_cmd(MSG_DUP, MSG_STDOUT_BACKUP, strerror(errno), false);
	if (io->fd_in != -1)
		if (dup2(io->fd_in, STDIN_FILENO) == -1)
			ret = errmsg_cmd(MSG_DUP2, io->infile, strerror(errno), false);
	if (io->fd_out != -1)
		if (dup2(io->fd_out, STDOUT_FILENO) == -1)
			ret = errmsg_cmd(MSG_DUP2, io->outfile, strerror(errno), false);
	return (ret);
}

// Verifica si los archivos de entrada y salida están correctamente configurados en io.
// Devuelve true si todo está bien, false si hay algún error con los descriptores.
bool check_infile_outfile(t_io_fds *io)
{
	if (!io || (!io->infile && !io->outfile))
		return (true); // Si no hay archivos, no hay nada que verificar
	if ((io->infile && io->fd_in == -1)
		|| (io->outfile && io->fd_out == -1))
		return (false); // Algún descriptor es inválido
	return (true);
} 