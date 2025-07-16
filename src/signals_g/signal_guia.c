#include "minishell.h"

// Handler para SIGINT en modo interactivo: limpia la línea y muestra un nuevo prompt
void	signal_reset_prompt(int signo)
{
	(void)signo; // No se usa el número de señal, pero se debe declarar
	write(1, "\n", 1); // Imprime un salto de línea en la terminal
	rl_on_new_line(); // Informa a readline que hay una nueva línea
	rl_replace_line("", 0); // Borra la línea actual de entrada
	rl_redisplay(); // Redibuja el prompt limpio
}

// Configura el manejo de señales para el modo interactivo (esperando input del usuario)
void	set_signals_interactive(void)
{
	struct sigaction	act; // Estructura para definir el comportamiento de la señal

	ignore_sigquit(); // Ignora SIGQUIT (Ctrl+\) en modo interactivo
	ft_memset(&act, 0, sizeof(act)); // Inicializa la estructura en cero
	act.sa_handler = &signal_reset_prompt; // Asigna el handler personalizado para SIGINT
	sigaction(SIGINT, &act, NULL); // Aplica el handler para SIGINT (Ctrl+C)
}

// Handler para SIGINT/SIGQUIT en modo no interactivo: solo mueve a nueva línea
void	signal_print_newline(int signal)
{
	(void)signal; // No se usa el número de señal
	rl_on_new_line(); // Mueve el cursor a una nueva línea
}

// Configura el manejo de señales para el modo no interactivo (procesando comandos)
void	set_signals_noninteractive(void)
{
	struct sigaction	act; // Estructura para definir el comportamiento de la señal

	ft_memset(&act, 0, sizeof(act)); // Inicializa la estructura en cero
	act.sa_handler = &signal_print_newline; // Asigna el handler para imprimir nueva línea
	sigaction(SIGINT, &act, NULL); // Aplica el handler para SIGINT (Ctrl+C)
	sigaction(SIGQUIT, &act, NULL); // Aplica el handler para SIGQUIT (Ctrl+\)
}

// Ignora la señal SIGQUIT (Ctrl+\) completamente
void	ignore_sigquit(void)
{
	struct sigaction	act; // Estructura para definir el comportamiento de la señal

	ft_memset(&act, 0, sizeof(act)); // Inicializa la estructura en cero
	act.sa_handler = SIG_IGN; // Asigna el handler de ignorar señal
	sigaction(SIGQUIT, &act, NULL); // Aplica la acción para SIGQUIT
} 