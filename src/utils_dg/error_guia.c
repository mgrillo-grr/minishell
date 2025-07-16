#include "minishell.h"

// Une dos cadenas de texto, liberando la anterior si es necesario.
// Si 'add' es NULL, retorna 'str'. Si 'str' es NULL, duplica 'add'.
// Si ambas existen, las concatena y libera la original.
char *join_strs(char *str, char *add)
{
	char *tmp;

	if (!add)
		return (str);
	if (!str)
		return (ft_strdup(add));
	tmp = str;
	str = ft_strjoin(tmp, add);
	free_ptr(tmp);
	return (str);
}

// Determina si se deben agregar comillas a los detalles del mensaje de error
// para los comandos export y unset.
static bool add_detail_quotes(char *command)
{
	if (ft_strncmp(command, MSG_EXPORT, 7) == 0
		|| ft_strncmp(command, MSG_UNSET, 6) == 0)
		return (true);
	return (false);
}

// Imprime un mensaje de error relacionado a un comando, con formato personalizado.
// Puede incluir el nombre del comando, detalles y el mensaje de error.
// Devuelve el número de error recibido como argumento.
int errmsg_cmd(char *command, char *detail, char *error_message, int error_nb)
{
	char *msg;
	bool detail_quotes;

	detail_quotes = add_detail_quotes(command);
	msg = ft_strdup(MSG_MINISHELL_PREFIX);
	if (command != NULL)
	{
		msg = join_strs(msg, command);
		msg = join_strs(msg, ": ");
	}
	if (detail != NULL)
	{
		if (detail_quotes)
			msg = join_strs(msg, "`");
		msg = join_strs(msg, detail);
		if (detail_quotes)
			msg = join_strs(msg, "'");
		msg = join_strs(msg, ": ");
	}
	msg = join_strs(msg, error_message);
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
	return (error_nb);
}

// Imprime un mensaje de error general (no asociado a un comando específico).
// Permite incluir detalles y decidir si se muestran entre comillas.
void errmsg(char *errmsg, char *detail, int quotes)
{
	char *msg;

	msg = ft_strdup(MSG_MINISHELL_PREFIX);
	msg = join_strs(msg, errmsg);
	if (quotes)
		msg = join_strs(msg, " `");
	else
		msg = join_strs(msg, ": ");
	msg = join_strs(msg, detail);
	if (quotes)
		msg = join_strs(msg, "'");
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
}

// Imprime el mensaje de uso de la minishell si los argumentos de inicio son inválidos.
// Devuelve el valor booleano recibido como argumento.
bool usage_message(bool return_val)
{
	ft_putendl_fd(USAGE_MINISHELL, 2);
	return (return_val);
} 