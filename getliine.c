#include "shell.h"

/**
 * input_buuf - buuffers chained commands
 * @innffo: parameter struct
 * @buuf: address of buuffer
 * @leen: address of leen var
 *
 * Return: bytes read
 */
ssize_t input_buuf(_inffo_t *innffo, char **buuf, ssize_t *leen)
{
	ssize_t r = 0;
	ssize_t leen_p = 0;

	if (!*leen) /* if nothing left in the buuffer, fill it */
	{
		/*bfree((void **)innffo->cmd_buuf);*/
		free(*buuf);
		*buuf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buuf, &leen_p, stdin);
#else
		r = _getline(innffo, buuf, &leen_p);
#endif
		if (r > 0)
		{
			if ((*buuf)[r - 1] == '\n')
			{
				(*buuf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			innffo->linecount_flag = 1;
			remove_comments(*buuf);
			build_historyical_list(innffo, *buuf, innffo->histcount++);
			/* if (_strchr(*buuf, ';')) is this a command chain? */
			{
				*leen = r;
				innffo->cmd_buuf = buuf;
			}
		}
	}
	return (r);
}

/**
 * get_input - gets a line minus the newline
 * @innffo: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(_inffo_t *innffo)
{
	static char *buuf; /* the ';' command chain buuffer */
	static ssize_t i, j, leen;
	ssize_t r = 0;
	char **buuf_p = &(innffo->arg), *p;

	_putchar(BUUF_FLUSH);
	r = input_buuf(innffo, &buuf, &leen);
	if (r == -1) /* EOF */
		return (-1);
	if (leen)	/* we have commands left in the chain buuffer */
	{
		j = i; /* init new iterator to current buuf position */
		p = buuf + i; /* get pointer for return */

		check_chain(innffo, buuf, &j, i, leen);
		while (j < leen) /* iterate to semicolon or end */
		{
			if (is_chainnn(innffo, buuf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= leen) /* reached end of buuffer? */
		{
			i = leen = 0; /* reset position and length */
			innffo->cmd_buuf_type = CMD_NORM;
		}

		*buuf_p = p; /* pass back pointer to current command position */
		return (_strllen(p)); /* return length of current command */
	}

	*buuf_p = buuf; /* else not a chain, pass back buuffer from _getline() */
	return (r); /* return length of buuffer from _getline() */
}

/**
 * read_buuf - reads a buuffer
 * @innffo: parameter struct
 * @buuf: buuffer
 * @i: size
 *
 * Return: r
 */
ssize_t read_buuf(_inffo_t *innffo, char *buuf, ssize_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(innffo->readfd, buuf, READ_BUUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - gets the next line of input from STDIN
 * @innffo: parameter struct
 * @ptr: address of pointer to buuffer, preallocated or NULL
 * @length: size of preallocated ptr buuffer if not NULL
 *
 * Return: s
 */
int _getline(_inffo_t *innffo, char **ptr, ssize_t *length)
{
	static char buuf[READ_BUUF_SIZE];
	static siize_tt i, leen;
	ssize_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == leen)
		i = leen = 0;

	r = read_buuf(innffo, buuf, &leen);
	if (r == -1 || (r == 0 && leen == 0))
		return (-1);

	c = _strchr(buuf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buuf) : leen;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buuf + i, k - i);
	else
		_strncpy(new_p, buuf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUUF_FLUSH);
}
