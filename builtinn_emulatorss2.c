#include "shell.h"

/**
 * _myhistorical - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @innffo: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myhistorical(_inffo_t *innffo)
{
	print_list(innffo->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @innffo: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(_inffo_t *innffo, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_nodde_at_index(&(innffo->alias),
		get_nodde_index(innffo->alias, nodde_starts_with(innffo->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @innffo: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(_inffo_t *innffo, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(innffo, str));

	unset_alias(innffo, str);
	return (add_nodde_end(&(innffo->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @nodde: the alias nodde
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(liist_t *nodde)
{
	char *p = NULL, *a = NULL;

	if (nodde)
	{
		p = _strchr(nodde->str, '=');
		for (a = nodde->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - mimics the alias builtin (man alias)
 * @innffo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myalias(_inffo_t *innffo)
{
	int i = 0;
	char *p = NULL;
	liist_t *nodde = NULL;

	if (innffo->argc == 1)
	{
		nodde = innffo->alias;
		while (nodde)
		{
			print_alias(nodde);
			nodde = nodde->next;
		}
		return (0);
	}
	for (i = 1; innffo->argv[i]; i++)
	{
		p = _strchr(innffo->argv[i], '=');
		if (p)
			set_alias(innffo, innffo->argv[i]);
		else
			print_alias(nodde_starts_with(innffo->alias, innffo->argv[i], '='));
	}

	return (0);
}
