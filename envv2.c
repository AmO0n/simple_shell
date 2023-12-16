#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @innffo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_environ(_inffo_t *innffo)
{
	if (!innffo->environ || innffo->env_changed)
	{
		innffo->environ = liist_to_strings(innffo->env);
		innffo->env_changed = 0;
	}

	return (innffo->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @innffo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenv(_inffo_t *innffo, char *var)
{
	liist_t *nodde = innffo->env;
	ssize_t i = 0;
	char *p;

	if (!nodde || !var)
		return (0);

	while (nodde)
	{
		p = starts_with(nodde->str, var);
		if (p && *p == '=')
		{
			innffo->env_changed = delete_nodde_at_index(&(innffo->env), i);
			i = 0;
			nodde = innffo->env;
			continue;
		}
		nodde = nodde->next;
		i++;
	}
	return (innffo->env_changed);
}

/**
 * _setenvvvv - Initialize a new environment variable,
 *             or modify an existing one
 * @innffo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setenvvvv(_inffo_t *innffo, char *var, char *value)
{
	char *buuf = NULL;
	liist_t *nodde;
	char *p;

	if (!var || !value)
		return (0);

	buuf = malloc(_strllen(var) + _strllen(value) + 2);
	if (!buuf)
		return (1);
	_strcpy(buuf, var);
	_strcat(buuf, "=");
	_strcat(buuf, value);
	nodde = innffo->env;
	while (nodde)
	{
		p = starts_with(nodde->str, var);
		if (p && *p == '=')
		{
			free(nodde->str);
			nodde->str = buuf;
			innffo->env_changed = 1;
			return (0);
		}
		nodde = nodde->next;
	}
	add_nodde_end(&(innffo->env), buuf, 0);
	free(buuf);
	innffo->env_changed = 1;
	return (0);
}
