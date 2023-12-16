#include "shell.h"

/**
 * is_chainnn - test is current char in buuffer is a chain delimeter
 * @innffo: the parameter struct
 * @buuf: the char buuffer
 * @p: address of current position in buuf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_chainnn(_inffo_t *innffo, char *buuf, ssize_t *p)
{
	ssize_t j = *p;

	if (buuf[j] == '|' && buuf[j + 1] == '|')
	{
		buuf[j] = 0;
		j++;
		innffo->cmd_buuf_type = CMD_OR;
	}
	else if (buuf[j] == '&' && buuf[j + 1] == '&')
	{
		buuf[j] = 0;
		j++;
		innffo->cmd_buuf_type = CMD_AND;
	}
	else if (buuf[j] == ';') /* found end of this command */
	{
		buuf[j] = 0; /* replace semicolon with null */
		innffo->cmd_buuf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @innffo: the parameter struct
 * @buuf: the char buuffer
 * @p: address of current position in buuf
 * @i: starting position in buuf
 * @leen: length of buuf
 *
 * Return: Void
 */
void check_chain(_inffo_t *innffo, char *buuf, ssize_t *p, ssize_t i, ssize_t leen)
{
	ssize_t j = *p;

	if (innffo->cmd_buuf_type == CMD_AND)
	{
		if (innffo->status)
		{
			buuf[i] = 0;
			j = leen;
		}
	}
	if (innffo->cmd_buuf_type == CMD_OR)
	{
		if (!innffo->status)
		{
			buuf[i] = 0;
			j = leen;
		}
	}

	*p = j;
}

/**
 * replace_alias - replaces an aliases in the tokenized string
 * @innffo: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(_inffo_t *innffo)
{
	int i;
	liist_t *nodde;
	char *p;

	for (i = 0; i < 10; i++)
	{
		nodde = nodde_starts_with(innffo->alias, innffo->argv[0], '=');
		if (!nodde)
			return (0);
		free(innffo->argv[0]);
		p = _strchr(nodde->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		innffo->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - replaces vars in the tokenized string
 * @innffo: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(_inffo_t *innffo)
{
	int i = 0;
	liist_t *nodde;

	for (i = 0; innffo->argv[i]; i++)
	{
		if (innffo->argv[i][0] != '$' || !innffo->argv[i][1])
			continue;

		if (!_strcmp(innffo->argv[i], "$?"))
		{
			replace_string(&(innffo->argv[i]),
				_strdup(convert_number(innffo->status, 10, 0)));
			continue;
		}
		if (!_strcmp(innffo->argv[i], "$$"))
		{
			replace_string(&(innffo->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		nodde = nodde_starts_with(innffo->env, &innffo->argv[i][1], '=');
		if (nodde)
		{
			replace_string(&(innffo->argv[i]),
				_strdup(_strchr(nodde->str, '=') + 1));
			continue;
		}
		replace_string(&innffo->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * replace_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
