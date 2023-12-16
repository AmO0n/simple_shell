#include "shell.h"

/**
 * _myenvv - prints the current environment
 * @innffo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int _myenvv(_inffo_t *innffo)
{
	print_list_str(innffo->env);
	return (0);
}

/**
 * _getenvv - gets the value of an environ variable
 * @innffo: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_getenvv(_inffo_t *innffo, const char *name)
{
	liist_t *nodde = innffo->env;
	char *p;

	while (nodde)
	{
		p = starts_with(nodde->str, name);
		if (p && *p)
			return (p);
		nodde = nodde->next;
	}
	return (NULL);
}

/**
 * _mysetenvv - Initialize a new environment variable,
 *             or modify an existing one
 * @innffo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _mysetenvv(_inffo_t *innffo)
{
	if (innffo->argc != 3)
	{
		_eputss("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenvvv(innffo, innffo->argv[1], innffo->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenvv - Remove an environment variable
 * @innffo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myunsetenvv(_inffo_t *innffo)
{
	int i;

	if (innffo->argc == 1)
	{
		_eputss("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= innffo->argc; i++)
		_unsetenv(innffo, innffo->argv[i]);

	return (0);
}

/**
 * populate_env_list - populates env linked list
 * @innffo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int populate_env_list(_inffo_t *innffo)
{
	liist_t *nodde = NULL;
	siize_tt i;

	for (i = 0; environ[i]; i++)
		add_nodde_end(&nodde, environ[i], 0);
	innffo->env = nodde;
	return (0);
}
