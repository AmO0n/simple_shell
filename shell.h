#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buuffers */
#define READ_BUUF_SIZE 1024
#define WRITE_BUUF_SIZE 1024
#define BUUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next nodde
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} liist_t;

/**
 *struct passinnffo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history nodde
 *@alias: the alias nodde
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buuf: address of pointer to cmd_buuf, on if chaining
 *@cmd_buuf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinnffo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	liist_t *env;
	liist_t *history;
	liist_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buuf; /* pointer to cmd ; chain buuffer, for memory mangement */
	int cmd_buuf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} _inffo_t;

#define INNFFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(_inffo_t *);
} builtin_table;


/* hssh.c */
int hssh(_inffo_t *, char **);
int find_builtin(_inffo_t *);
void find_cmd(_inffo_t *);
void forkk_cmd(_inffo_t *);

/* path.c */
int is_cmd(_inffo_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(_inffo_t *, char *, char *);

/* loophssh.c */
int loophssh(char **);

/* err_stringg_functions.c */
void _eputss(char *);
int _eputchar(char);
int _putsfddd(char c, int fd);
int _putsfd(char *str, int fd);

/* strring_functions.c */
int _strllen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* strring_functions2.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* strring_functions3.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* strring_functions4.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* memmory_functions */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* memmory_functions2.c */
int bfree(void **);

/* morre_functions.c */
int interactive(_inffo_t *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* morre_functions2.c */
int _erratoi(char *);
void print_error(_inffo_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* builtinn_emulatorss.c */
int _myeexit(_inffo_t *);
int _maycd(_inffo_t *);
int _myhelp(_inffo_t *);

/* builtinn_emulatorss2.c */
int _myhistorical(_inffo_t *);
int _myalias(_inffo_t *);

/* getliine.c module */
ssize_t get_input(_inffo_t *);
int _getline(_inffo_t *, char **, siize_tt *);
void sigintHandler(int);

/* innffoo.c module */
void clear_innffo(_inffo_t *);
void set_innffo(_inffo_t *, char **);
void free_innffo(_inffo_t *, int);

/* envv.c module */
char *_getenvv(_inffo_t *, const char *);
int _myenvv(_inffo_t *);
int _mysetenvv(_inffo_t *);
int _myunsetenvv(_inffo_t *);
int populate_env_list(_inffo_t *);

/* envv2.c module */
char **get_environ(_inffo_t *);
int _unsetenv(_inffo_t *, char *);
int _setenvvvv(_inffo_t *, char *, char *);

/* file_ioo_functions.c */
char *get_historical_file(_inffo_t *innffo);
int write_historical(_inffo_t *innffo);
int read_historical(_inffo_t *innffo);
int build_historyical_list(_inffo_t *innffo, char *buuf, int linecount);
int renumber_historical(_inffo_t *innffo);

/* liststrr.c module */
liist_t *add_nodde(liist_t **, const char *, int);
liist_t *add_nodde_end(liist_t **, const char *, int);
siize_tt print_list_str(const liist_t *);
int delete_nodde_at_index(liist_t **, unsigned int);
void free_list(liist_t **);

/* liststrr2.c module */
siize_tt list_leen(const liist_t *);
char **liist_to_strings(liist_t *);
siize_tt print_list(const liist_t *);
liist_t *nodde_starts_with(liist_t *, char *, char);
ssize_t get_nodde_index(liist_t *, liist_t *);

/* chainn.c */
int is_chainnn(_inffo_t *, char *, siize_tt *);
void check_chain(_inffo_t *, char *, siize_tt *, siize_tt, siize_tt);
int replace_alias(_inffo_t *);
int replace_vars(_inffo_t *);
int replace_string(char **, char *);

#endif
