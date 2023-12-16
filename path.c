#include "shell.h"

/**
 * is_cmd - determines if a file is an executable command
 * @innffo: the innffo struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_cmd(_inffo_t *innffo, char *path)
{
	struct stat st;

	(void)innffo;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dup_chars - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buuffer
 */
char *dup_chars(char *pathstr, int start, int stop)
{
	static char buuf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buuf[k++] = pathstr[i];
	buuf[k] = 0;
	return (buuf);
}

/**
 * find_path - finds this cmd in the PATH string
 * @innffo: the innffo struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *find_path(_inffo_t *innffo, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strllen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (is_cmd(innffo, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dup_chars(pathstr, curr_pos, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (is_cmd(innffo, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
