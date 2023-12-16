#include "shell.h"

/**
 * get_historical_file - gets the history file
 * @innffo: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_historical_file(_inffo_t *innffo)
{
	char *buuf, *dir;

	dir = _getenvv(innffo, "HOME=");
	if (!dir)
		return (NULL);
	buuf = malloc(sizeof(char) * (_strllen(dir) + _strllen(HIST_FILE) + 2));
	if (!buuf)
		return (NULL);
	buuf[0] = 0;
	_strcpy(buuf, dir);
	_strcat(buuf, "/");
	_strcat(buuf, HIST_FILE);
	return (buuf);
}

/**
 * write_historical - creates a file, or appends to an existing file
 * @innffo: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_historical(_inffo_t *innffo)
{
	ssize_t fd;
	char *filename = get_historical_file(innffo);
	liist_t *nodde = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (nodde = innffo->history; nodde; nodde = nodde->next)
	{
		_putsfd(nodde->str, fd);
		_putsfd('\n', fd);
	}
	_putsfd(BUUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_historical - reads history from file
 * @innffo: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_historical(_inffo_t *innffo)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buuf = NULL, *filename = get_historical_file(innffo);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buuf = malloc(sizeof(char) * (fsize + 1));
	if (!buuf)
		return (0);
	rdlen = read(fd, buuf, fsize);
	buuf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buuf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buuf[i] == '\n')
		{
			buuf[i] = 0;
			build_historyical_list(innffo, buuf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_historyical_list(innffo, buuf + last, linecount++);
	free(buuf);
	innffo->histcount = linecount;
	while (innffo->histcount-- >= HIST_MAX)
		delete_nodde_at_index(&(innffo->history), 0);
	renumber_historical(innffo);
	return (innffo->histcount);
}

/**
 * build_historyical_list - adds entry to a history linked list
 * @innffo: Structure containing potential arguments. Used to maintain
 * @buuf: buuffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int build_historyical_list(_inffo_t *innffo, char *buuf, int linecount)
{
	liist_t *nodde = NULL;

	if (innffo->history)
		nodde = innffo->history;
	add_nodde_end(&nodde, buuf, linecount);

	if (!innffo->history)
		innffo->history = nodde;
	return (0);
}

/**
 * renumber_historical - renumbers the history linked list after changes
 * @innffo: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_historical(_inffo_t *innffo)
{
	liist_t *nodde = innffo->history;
	int i = 0;

	while (nodde)
	{
		nodde->num = i++;
		nodde = nodde->next;
	}
	return (innffo->histcount = i);
}
