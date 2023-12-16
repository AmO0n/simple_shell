#include "shell.h"

/**
 * list_leen - determines length of linked list
 * @h: pointer to first nodde
 *
 * Return: size of list
 */
siize_tt list_leen(const liist_t *h)
{
	siize_tt i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * liist_to_strings - returns an array of strings of the list->str
 * @head: pointer to first nodde
 *
 * Return: array of strings
 */
char **liist_to_strings(liist_t *head)
{
	liist_t *nodde = head;
	siize_tt i = list_leen(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; nodde; nodde = nodde->next, i++)
	{
		str = malloc(_strllen(nodde->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _strcpy(str, nodde->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * print_list - prints all elements of a liist_t linked list
 * @h: pointer to first nodde
 *
 * Return: size of list
 */
siize_tt print_list(const liist_t *h)
{
	siize_tt i = 0;

	while (h)
	{
		_puts(convert_number(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * nodde_starts_with - returns nodde whose string starts with prefix
 * @nodde: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match nodde or null
 */
liist_t *nodde_starts_with(liist_t *nodde, char *prefix, char c)
{
	char *p = NULL;

	while (nodde)
	{
		p = starts_with(nodde->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (nodde);
		nodde = nodde->next;
	}
	return (NULL);
}

/**
 * get_nodde_index - gets the index of a nodde
 * @head: pointer to list head
 * @nodde: pointer to the nodde
 *
 * Return: index of nodde or -1
 */
ssize_t get_nodde_index(liist_t *head, liist_t *nodde)
{
	siize_tt i = 0;

	while (head)
	{
		if (head == nodde)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}
