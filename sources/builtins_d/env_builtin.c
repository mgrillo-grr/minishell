

#include "minishell.h"

/* env_builtin:
*	Executes the builtin env command: Prints the environment variables.
*/
int	env_builtin(t_data *data, char **args)
{
	int	i;

	if (args && args[1])
		return (errmsg_cmd(MSG_ENV, NULL, E_TOO_MANY_ARGUMENTS, 2));
	i = 0;
	if (!data->env)
		return (EXIT_FAILURE);
	while (data->env[i])
		ft_putendl_fd(data->env[i++], STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
