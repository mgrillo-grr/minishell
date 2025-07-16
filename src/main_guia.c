
#include "minishell.h"

// Función para verificar argumentos al iniciar la shell
static bool start_check(t_data *data, int ac, char **av)
{
	(void)data; // No se usa, pero se mantiene la firma
	(void)av;   // No se usa, pero se mantiene la firma
	if (ac != 1)
		return (usage_message(false)); // Si hay argumentos extra, muestra mensaje de uso
	return (true); // Todo correcto
}

// Bucle principal de la minishell en modo interactivo
void minishell_start(t_data *data)
{
	while (1) // Bucle infinito hasta que el usuario salga
	{
		set_signals_interactive(); // Configura señales para modo interactivo (esperando input)
		data->user_input = readline(PROMPT); // Muestra el prompt y lee el comando del usuario
		set_signals_noninteractive(); // Configura señales para modo no interactivo (procesando comando)
		if (parse_user_input(data) == true) // Analiza el comando ingresado
			g_last_exit_code = execute(data); // Si es válido, ejecuta y guarda el exit code
		else
			g_last_exit_code = 1; // Si es inválido, exit code de error
		free_data(data, false); // Libera memoria asociada al comando actual
	}
}

// Función principal del programa
int main(int ac, char **av, char **env)
{
	t_data data; // Estructura principal de datos de la shell

	ft_memset(&data, 0, sizeof(t_data)); // Inicializa la estructura en cero
	if (!start_check(&data, ac, av) || !init_data(&data, env))
		exit_shell(NULL, EXIT_FAILURE); // Si falla la verificación o inicialización, sale con error
	minishell_start(&data); // Inicia el bucle principal de la shell
	exit_shell(&data, g_last_exit_code); // Sale de la shell con el último exit code
	return (0); // Fin del programa
}
