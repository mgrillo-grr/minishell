/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdios-el <mdios-el@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 10:00:00 by mdios-el          #+#    #+#             */
/*   Updated: 2024/07/26 10:00:00 by mdios-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	init_env(t_data *data, char **env)
{
	int		i;

	data->env = ft_calloc(env_var_count(env) + 1, sizeof * data->env);
	if (!data->env)
		return (false);
	i = 0;
	while (env[i])
	{
		data->env[i] = ft_strdup(env[i]);
		if (!data->env[i])
			return (false);
		i++;
	}
	return (true);
}

static bool	init_wds(t_data *data)
{
	char	buff[PATH_MAX];
	char	*wd;

	wd = getcwd(buff, PATH_MAX);
	data->working_dir = ft_strdup(wd);
	if (!data->working_dir)
		return (false);
	if (get_env_var_index(data->env, "OLDPWD") != -1)
	{
		data->old_working_dir = ft_strdup(get_env_var_value(data->env,
					"OLDPWD"));
		if (!data->old_working_dir)
			return (false);
	}
	else
	{
		data->old_working_dir = ft_strdup(wd);
		if (!data->old_working_dir)
			return (false);
	}
	return (true);
}

bool	init_data(t_data *data, char **env)
{
	if (!init_env(data, env))
	{
		errmsg_cmd(MSG_FATAL, NULL, E_INIT_ENV, 1);
		return (false);
	}
	if (!init_wds(data))
	{
		errmsg_cmd(MSG_FATAL, NULL, E_INIT_WDS, 1);
		return (false);
	}
	data->token = NULL;
	data->user_input = NULL;
	data->cmd = NULL;
	data->pid = -1;
	g_last_exit_code = 0;
	return (true);
}

void	init_io(t_command *cmd)
{
	if (!cmd->io_fds)
	{
		cmd->io_fds = malloc(sizeof * cmd->io_fds);
		if (!cmd->io_fds)
			return ;
		cmd->io_fds->infile = NULL;
		cmd->io_fds->outfile = NULL;
		cmd->io_fds->heredoc_delimiter = NULL;
		cmd->io_fds->heredoc_quotes = false;
		cmd->io_fds->fd_in = -1;
		cmd->io_fds->fd_out = -1;
		cmd->io_fds->stdin_backup = -1;
		cmd->io_fds->stdout_backup = -1;
	}
}
