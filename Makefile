CC = cc 
CFLAGS = -Wall -Wextra -Werror 
SRC_PARSING=./parsing/tools/split.c ./parsing/tools/string_pasing.c ./parsing/tools/free_file.c ./parsing/parsing.c main.c

FLAGS= -lreadline
OBJ_PARSING = $(SRC_PARSING:.c=.o)

NAME = minishell

all : $(NAME)

$(NAME) : $(OBJ_PARSING)
	$(CC) $(CFLAGS) ${OBJ_PARSING} -o $@ -lreadline

%.o : %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -rf $(OBJ_PARSING)

fclean : clean
	rm -rf $(NAME)

re : fclean all