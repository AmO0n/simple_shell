#include "shell.h"

/**
 *_eputss - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _eputss(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_eputchar(str[i]);
		i++;
	}
}

/**
 * _eputchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _eputchar(char c)
{
	static int i;
	static char buuf[WRITE_BUUF_SIZE];

	if (c == BUUF_FLUSH || i >= WRITE_BUUF_SIZE)
	{
		write(2, buuf, i);
		i = 0;
	}
	if (c != BUUF_FLUSH)
		buuf[i++] = c;
	return (1);
}

/**
 * _putsfddd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putsfddd(char c, int fd)
{
	static int i;
	static char buuf[WRITE_BUUF_SIZE];

	if (c == BUUF_FLUSH || i >= WRITE_BUUF_SIZE)
	{
		write(fd, buuf, i);
		i = 0;
	}
	if (c != BUUF_FLUSH)
		buuf[i++] = c;
	return (1);
}

/**
 *_putsfd - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int _putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _putsfddd(*str++, fd);
	}
	return (i);
}
