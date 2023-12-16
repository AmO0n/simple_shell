#include "shell.h"

/**
 * clear_innffo - initializes _inffo_t struct
 * @innffo: struct address
 */
void clear_innffo(_inffo_t *innffo)
{
	innffo->arg = NULL;
	innffo->argv = NULL;
	innffo->path = NULL;
	innffo->argc = 0;
}

/**
 * set_innffo - initializes _inffo_t struct
 * @innffo: struct address
 * @av: argument vector
 */
void set_innffo(_inffo_t *innffo, char **av)
{
	int i = 0;

	innffo->fname = av[0];
	if (innffo->arg)
	{
		innffo->argv = strtow(innffo->arg, " \t");
		if (!innffo->argv)
		{

			innffo->argv = malloc(sizeof(char *) * 2);
			if (innffo->argv)
			{
				innffo->argv[0] = _strdup(innffo->arg);
				innffo->argv[1] = NULL;
			}
		}
		for (i = 0; innffo->argv && innffo->argv[i]; i++)
			;
		innffo->argc = i;

		replace_alias(innffo);
		replace_vars(innffo);
	}
}

/**
 * free_innffo - frees _inffo_t struct fields
 * @innffo: struct address
 * @all: true if freeing all fields
 */
void free_innffo(_inffo_t *innffo, int all)
{
	ffree(innffo->argv);
	innffo->argv = NULL;
	innffo->path = NULL;
	if (all)
	{
		if (!innffo->cmd_buuf)
			free(innffo->arg);
		if (innffo->env)
			free_list(&(innffo->env));
		if (innffo->history)
			free_list(&(innffo->history));
		if (innffo->alias)
			free_list(&(innffo->alias));
		ffree(innffo->environ);
			innffo->environ = NULL;
		bfree((void **)innffo->cmd_buuf);
		if (innffo->readfd > 2)
			close(innffo->readfd);
		_putchar(BUUF_FLUSH);
	}
}
