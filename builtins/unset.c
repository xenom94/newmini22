/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:27:07 by nel-ouar          #+#    #+#             */
/*   Updated: 2024/11/25 15:20:28 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	length(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

static int	find_env_var(char *var_name)
{
	int		i;
	size_t	var_len;
	size_t	curr_len;

	i = 0;
	if (!var_name)
		return (-1);
	var_len = length(var_name);
	while (g_vars.env[i])
	{
		curr_len = length(g_vars.env[i]);
		if (curr_len == var_len && !ft_strncmp(g_vars.env[i], var_name,
				var_len))
			return (i);
		i++;
	}
	return (-1);
}

static void	remove_env_var(int index, int env_len)
{
	int	i;

	if (index < 0 || index >= env_len)
		return ;
	i = index;
	while (i < env_len - 1)
	{
		g_vars.env[i] = g_vars.env[i + 1];
		i++;
	}
	g_vars.env[env_len - 1] = NULL;
}

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	add_env_var(const char *new_var)
{
	int		env_len;
	char	**new_env;
	int		i;

	env_len = 0;
	while (g_vars.env[env_len])
		env_len++;
	new_env = malloc(sizeof(char *) * (env_len + 2));
	if (!new_env)
		return ;
	i = 0;
	while (i < env_len)
	{
		new_env[i] = g_vars.env[i];
		i++;
	}
	new_env[env_len] = ft_strdup(new_var);
	new_env[env_len + 1] = NULL;
	g_vars.env = new_env;
}

void	unset(t_command *cmd)
{
	int	i;
	int	env_index;
	int	env_len;

	i = 0 ;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i++]))
		{
			ft_printf("minishell: unset: `%s': not a valid identifier\n",
				cmd->args[i++]);
			g_vars.exit_status = 1;
			continue ;
		}
		env_len = 0;
		while (g_vars.env[env_len])
			env_len++;
		env_index = find_env_var(cmd->args[i]);
		if (env_index != -1)
		{
			remove_env_var(env_index, env_len);
			if (ft_strcmp(cmd->args[i++], "SHLVL") == 0)
				add_env_var("SHLVL=1");
		}
	}
}
