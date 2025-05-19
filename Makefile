#Executable name
NAME = pipex

#Compilator and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

#Variables
FILES = main.c pipex_utils.c
OBJECTS = $(FILES:.c=.o)
INPUT_FILE = infile

#Library path
LIBDIR = ./libft
LIBFT = $(LIBDIR)/libft.a

#Default rule: compile all
all: $(LIBFT) $(NAME)

#Rule to create the program
$(NAME): $(OBJECTS) $(LIBFT) $(INPUT_FILE)
	$(CC) $(CFLAGS) $(OBJECTS) -L$(LIBDIR) -lft -o $(NAME)

#Rule to create the needed library
$(LIBFT):
	make -C $(LIBDIR) all

$(INPUT_FILE):
	touch $(INPUT_FILE)

#Auto compile .c into .o
%.o: %.c push_swap.h $(LIBDIR)/libft.h
	$(CC) $(CFLAGS) -c $< -o $@

#Rule to clean all the created files
clean:
	$(MAKE) -C $(LIBDIR) clean
	rm -f $(OBJECTS)

#Rule to clean all, including the library
fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	rm -f $(NAME)

#Rule to recompile all
re: fclean all

#Indicates that the next rules are not files
.PHONY: all clean fclean re