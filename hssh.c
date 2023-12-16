#include "shell.h"

/**
 * hssh - main shell loop
 * @innffo: the parameter & return innffo struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hssh(_inffo_t *innffo, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_innffo(innffo);
		if (interactive(innffo))
			_puts("$ ");
		_eputchar(BUUF_FLUSH);
		r = get_input(innffo);
		if (r != -1)
		{
			set_innffo(innffo, av);
			builtin_ret = find_builtin(innffo);
			if (builtin_ret == -1)
				find_cmd(innffo);
		}
		else if (interactive(innffo))
			_putchar('\n');
		free_innffo(innffo, 0);
	}
	write_historical(innffo);
	free_innffo(innffo, 1);
	if (!interactive(innffo) && innffo->status)
		exit(innffo->status);
	if (builtin_ret == -2)
	{
		if (innffo->err_num == -1)
			exit(innffo->status);
		exit(innffo->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @innffo: the parameter & return innffo struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_builtin(_inffo_t *innffo)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myeexit},
		{"env", _myenvv},
		{"help", _myhelp},
		{"history", _myhistorical},
		{"setenv", _mysetenvv},
		{"unsetenv", _myunsetenvv},
		{"cd", _maycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(innffo->argv[0], builtintbl[i].type) == 0)
		{
			innffo->line_count++;
			built_in_ret = builtintbl[i].func(innffo);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @innffo: the parameter & return innffo struct
 *
 * Return: void
 */
void find_cmd(_inffo_t *innffo)
{
	char *path = NULL;
	int i, k;

	innffo->path = innffo->argv[0];
	if (innffo->linecount_flag == 1)
	{
		innffo->line_count++;
		innffo->linecount_flag = 0;
	}
	for (i = 0, k = 0; innffo->arg[i]; i++)
		if (!is_delim(innffo->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(innffo, _getenvv(innffo, "PATH="), innffo->argv[0]);
	if (path)
	{
		innffo->path = path;
		forkk_cmd(innffo);
	}
	else
	{
		if ((interactive(innffo) || _getenvv(innffo, "PATH=")
			|| innffo->argv[0][0] == '/') && is_cmd(innffo, innffo->argv[0]))
			fork_cmd(innffo);
		else if (*(innffo->arg) != '\n')
		{
			innffo->status = 127;
			print_error(innffo, "not found\n");
		}
	}
}

/**
 * forkk_cmd - forks a an exec thread to run cmd
 * @innffo: the parameter & return innffo struct
 *
 * Return: void
 */
void forkk_cmd(_inffo_t *innffo)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(innffo->path, innffo->argv, get_environ(innffo)) == -1)
		{
			free_innffo(innffo, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(innffo->status));
		if (WIFEXITED(innffo->status))
		{
			innffo->status = WEXITSTATUS(innffo->status);
			if (innffo->status == 126)
				print_error(innffo, "Permission denied\n");
		}
	}
}
