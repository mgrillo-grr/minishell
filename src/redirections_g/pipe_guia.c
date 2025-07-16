#include "minishell.h"

// Cierra los descriptores de los pipes de todos los comandos, excepto el comando skip_cmd si se indica.
void close_pipe_fds(t_command *cmds, t_command *skip_cmd)
{
	while (cmds)
	{
		if (cmds != skip_cmd && cmds->pipe_fd)
		{
			close(cmds->pipe_fd[0]); // Cierra el extremo de lectura del pipe
			close(cmds->pipe_fd[1]); // Cierra el extremo de escritura del pipe
		}
		cmds = cmds->next;
	}
}

// Crea los pipes necesarios para la ejecución de los comandos encadenados (pipeline).
// Asigna los descriptores de pipe a cada comando que lo requiera.
// Si ocurre un error, libera la memoria y retorna false.
bool create_pipes(t_data *data)
{
	int *fd;
	t_command *tmp;

	tmp = data->cmd;
	while (tmp)
	{
		if (tmp->pipe_output || (tmp->prev && tmp->prev->pipe_output))
		{
			fd = malloc(sizeof * fd * 2); // Reserva espacio para los dos extremos del pipe
			if (!fd || pipe(fd) != 0)
			{
				free_data(data, false); // Libera recursos en caso de error
				return (false);
			}
			tmp->pipe_fd = fd; // Asigna el pipe al comando
		}
		tmp = tmp->next;
	}
	return (true);
}

// Configura los descriptores de archivo para los pipes de un comando específico.
// Redirige la entrada/salida estándar según corresponda y cierra los pipes innecesarios.
bool set_pipe_fds(t_command *cmds, t_command *c)
{
	if (!c)
		return (false);
	if (c->prev && c->prev->pipe_output)
		dup2(c->prev->pipe_fd[0], STDIN_FILENO); // Redirige stdin al extremo de lectura del pipe anterior
	if (c->pipe_output)
		dup2(c->pipe_fd[1], STDOUT_FILENO); // Redirige stdout al extremo de escritura del pipe actual
	close_pipe_fds(cmds, c); // Cierra los pipes que no se usan en este comando
	return (true);
} 