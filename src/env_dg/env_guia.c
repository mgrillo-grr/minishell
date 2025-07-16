#include "minishell.h"

// Cuenta la cantidad de variables de entorno en el array env.
// Devuelve el número de variables (no incluye el puntero NULL final).
int env_var_count(char **env)
{
	int i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

// Busca el índice de una variable de entorno (por clave) en el array env.
// Devuelve el índice si la encuentra, o -1 si no existe.
int get_env_var_index(char **env, char *var)
{
	int i;
	char *tmp;

	tmp = ft_strjoin(var, "="); // Prepara la clave con '='
	if (!tmp)
		return (-1);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
		{
			free_ptr(tmp);
			return (i); // Encontrada
		}
		i++;
	}
	free_ptr(tmp);
	return (-1); // No encontrada
}

// Obtiene el valor de una variable de entorno (por clave) en el array env.
// Devuelve un puntero al valor (después del '=') o NULL si no existe.
char *get_env_var_value(char **env, char *var)
{
	int i;
	char *tmp;

	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
		{
			free_ptr(tmp);
			return (ft_strchr(env[i], '=') + 1); // Retorna el valor
		}
		i++;
	}
	free_ptr(tmp);
	return (NULL);
}

// Verifica si una clave de variable de entorno es válida (alfanumérica o '_', no empieza con número).
// Devuelve true si es válida, false si no lo es.
bool is_valid_env_var_key(char *var)
{
	int i;

	i = 0;
	if (ft_isalpha(var[i]) == 0 && var[i] != '_')
		return (false);
	i++;
	while (var[i] && var[i] != '=')
	{
		if (ft_isalnum(var[i]) == 0 && var[i] != '_')
			return (false);
		i++;
	}
	return (true);
} 