#include "shell.h"

/**
 * add_nodde - adds a nodde to the start of the list
 * @head: address of pointer to head nodde
 * @str: str field of nodde
 * @num: nodde index used by history
 *
 * Return: size of list
 */
liist_t *add_nodde(liist_t **head, const char *str, int num)
{
	liist_t *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(liist_t));
	if (!new_head)
		return (NULL);
	_memset((void *)new_head, 0, sizeof(liist_t));
	new_head->num = num;
	if (str)
	{
		new_head->str = _strdup(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

/**
 * add_nodde_end - adds a nodde to the end of the list
 * @head: address of pointer to head nodde
 * @str: str field of nodde
 * @num: nodde index used by history
 *
 * Return: size of list
 */
liist_t *add_nodde_end(liist_t **head, const char *str, int num)
{
	liist_t *new_nodde, *nodde;

	if (!head)
		return (NULL);

	nodde = *head;
	new_nodde = malloc(sizeof(liist_t));
	if (!new_nodde)
		return (NULL);
	_memset((void *)new_nodde, 0, sizeof(liist_t));
	new_nodde->num = num;
	if (str)
	{
		new_nodde->str = _strdup(str);
		if (!new_nodde->str)
		{
			free(new_nodde);
			return (NULL);
		}
	}
	if (nodde)
	{
		while (nodde->next)
			nodde = nodde->next;
		nodde->next = new_nodde;
	}
	else
		*head = new_nodde;
	return (new_nodde);
}

/**
 * print_list_str - prints only the str element of a liist_t linked list
 * @h: pointer to first nodde
 *
 * Return: size of list
 */
siize_tt print_list_str(const liist_t *h)
{
	siize_tt i = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * delete_nodde_at_index - deletes nodde at given index
 * @head: address of pointer to first nodde
 * @index: index of nodde to delete
 *
 * Return: 1 on success, 0 on failure
 */
int delete_nodde_at_index(liist_t **head, unsigned int index)
{
	liist_t *nodde, *prev_nodde;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		nodde = *head;
		*head = (*head)->next;
		free(nodde->str);
		free(nodde);
		return (1);
	}
	nodde = *head;
	while (nodde)
	{
		if (i == index)
		{
			prev_nodde->next = nodde->next;
			free(nodde->str);
			free(nodde);
			return (1);
		}
		i++;
		prev_nodde = nodde;
		nodde = nodde->next;
	}
	return (0);
}

/**
 * free_list - frees all noddes of a list
 * @head_ptr: address of pointer to head nodde
 *
 * Return: void
 */
void free_list(liist_t **head_ptr)
{
	liist_t *nodde, *next_nodde, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	nodde = head;
	while (nodde)
	{
		next_nodde = nodde->next;
		free(nodde->str);
		free(nodde);
		nodde = next_nodde;
	}
	*head_ptr = NULL;
}
