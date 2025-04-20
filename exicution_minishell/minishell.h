#ifndef MINISHELL_H
#define MINISHELL_H
///the headar of all file that we will use in this project 
# include <fcntl.h> // 
# include <stdio.h>  // perror()
# include <readline/readline.h> //for 
# include <readline/history.h> // for readline
# include <unistd.h> //  for getcwd() , chdir() , isatty()   q
# include <sys/stat.h> // stat() & lstat() & fstat()
# include <sys/types.h> // opendir() closedir()
# include <dirent.h> // opendir() readdir() closedir()
# include <string.h> // strerror() && 
# include <sys/ioctl.h> //
# include <stdlib.h> //
# include <termios.h> //
# include <curses.h> //
# include <term.h> //
# include <limits.h>

char	*find_path(char *cmd, char **env);

#endif