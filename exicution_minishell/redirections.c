#include "minishell.h"

static int open_file(char  *file, int mode)
{
    int fd;
    if(mode == 0)
        fd = open(file, O_RDONLY);
    else if (mode == 1)
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (mode == 2)
        fd = open (file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    return (fd);
}
//read in file <
void rederect_input (char *arg)
{
    int fd;
    fd = open_file(arg,0);
    if (fd == -1)
    {
        close(fd);
        exit(1);
    }
    if(!dup2(fd,0) == -1)
    {
        close(fd);
        exit(1);
    }
}
// write in file >
void rederect_output(char *arg)
{
    int fd;
    fd = open_file(arg,1);
    if (fd == -1)
    {
        close(fd);
        exit(1);

    }
    if(!dup2(fd, 1) == -1)
    {
        close(fd);
        exit(1);
    }
}
// add in file >>

void rederect_add_to_file(char *arg)
{
    int fd;
    fd = open_file(arg, 2);
    if (fd == -1)
    {
        close(fd);
        exit(1);
    }
    if(dup2 (fd, 1) == -1)
    {
        close(fd);
        exit(1);
    }
}
void herdoc (char *arg)
{
    while(1)
    {
        
    }
}
void redirections(char **arg)
{
    int i = 0;
    while (arg[i])
    {
        if (ft_strcmp(arg[i] , "<") == 0)
            rederect_input(arg[i+1]);
        else if (ft_strcmp(arg[i] , ">") == 0)
            rederect_output(arg[i+1]);
        else if (ft_strcmp(arg[i] , ">>") == 0)
            rederect_add_to_file(arg[i+1]);
        else if (ft_strcmp(arg[i], "<<") == 0)
            herdoc(arg[i+1]);
        i++;
    }
}