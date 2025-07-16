#include "minishell.h"

// Reserva un nuevo array de variables de entorno con el tamaño indicado y copia las existentes.
// Libera el array anterior y retorna el nuevo. Devuelve NULL si falla la reserva.
static char **realloc_env_vars(t_data *data, int size)
{
	char **new_env;
	int i;

	new_env = ft_calloc(size + 1, sizeof * new_env); // +1 para el NULL final
	if (!new_env)
		return (NULL);
	i = 0;
	while (data->env[i] && i < size)
	{
		new_env[i] = ft_strdup(data->env[i]); // Copia cada variable
		free_ptr(data->env[i]);
		i++;
	}
	free(data->env); // Libera el array anterior
	return (new_env);
}

// Establece o actualiza una variable de entorno (key, value) en la estructura de datos.
// Si la variable existe, la reemplaza; si no, la agrega al final.
// Devuelve true si tuvo éxito, false si hubo error de memoria.
bool set_env_var(t_data *data, char *key, char *value)
{
	int idx;
	char *tmp;

	idx = get_env_var_index(data->env, key);
	if (value == NULL)
		value = "";
	tmp = ft_strjoin("=", value); // Prepara el string '=valor'
	if (!tmp)
		return (false);
	if (idx != -1 && data->env[idx])
	{
		free_ptr(data->env[idx]);
		data->env[idx] = ft_strjoin(key, tmp); // Reemplaza la variable existente
	}
	else
	{
		idx = env_var_count(data->env);
		data->env = realloc_env_vars(data, idx + 1); // Amplía el array
		if (!data->env)
			return (false);
		data->env[idx] = ft_strjoin(key, tmp); // Agrega la nueva variable
	}
	free_ptr(tmp);
	return (true);
}

// Elimina una variable de entorno por índice del array env.
// Reordena el array y libera la memoria correspondiente.
// Devuelve true si tuvo éxito, false si hubo error.
bool remove_env_var(t_data *data, int idx)
{
	int i;
	int count;

	if (idx > env_var_count(data->env))
		return (false);
	free_ptr(data->env[idx]); // Libera la variable a eliminar
	i = idx;
	count = idx;
	while (data->env[i + 1])
	{
		data->env[i] = ft_strdup(data->env[i + 1]); // Desplaza las siguientes
		free_ptr(data->env[i + 1]);
		count++;
		i++;
	}
	data->env = realloc_env_vars(data, count); // Redimensiona el array
	if (!data->env)
		return (false);
	return (true);
} 