#include "shell.h"

/**
 * _myeexit - exits the shell
 * @innffo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if innffo.argv[0] != "exit"
 */
int _myeexit(_inffo_t *innffo)
{
	int exitcheck;

	if (innffo->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = _erratoi(innffo->argv[1]);
		if (exitcheck == -1)
		{
			innffo->status = 2;
			print_error(innffo, "Illegal number: ");
			_eputss(innffo->argv[1]);
			_eputchar('\n');
			return (1);
		}
		innffo->err_num = _erratoi(innffo->argv[1]);
		return (-2);
	}
	innffo->err_num = -1;
	return (-2);
}

/**
 * _maycd - changes the current directory of the process
 * @innffo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _maycd(_inffo_t *innffo)
{
	char *s, *dir, buuffer[1024];
	int chdir_rett;

	s = getcwd(buuffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!innffo->argv[1])
	{
		dir = _getenvv(innffo, "HOME=");
		if (!dir)
			chdir_rett = /* TODO: what should this be? */
				chdir((dir = _getenvv(innffo, "PWD=")) ? dir : "/");
		else
			chdir_rett = chdir(dir);
	}
	else if (_strcmp(innffo->argv[1], "-") == 0)
	{
		if (!_getenvv(innffo, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenvv(innffo, "OLDPWD=")), _putchar('\n');
		chdir_rett = /* TODO: what should this be? */
			chdir((dir = _getenvv(innffo, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_rett = chdir(innffo->argv[1]);
	if (chdir_rett == -1)
	{
		print_error(innffo, "CAN NOT cd to ");
		_eputss(innffo->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenvvvv(innffo, "OLDPWD", _getenvv(innffo, "PWD="));
		_setenvvv(innffo, "PWD", getcwd(buuffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes the current directory of the process
 * @innffo: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myhelp(_inffo_t *innffo)
{
	char **arrg_array;

	arrg_array = innffo->argv;
	_puts("help can call works. Functionis  not yet implemennted \n");
	if (0)
		_puts(*arrg_array); /* temp att_unused workaround */
	return (0);
}
