#Executable name
NAME = pipex

#Compilator and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

#Variables
FILES = main.c pipex_utils.c
OBJECTS = $(FILES:.c=.o)
B_FILES = main_bonus.c pipex_utils_bonus.c pipex_extra_bonus.c
B_OBJECTS = $(B_FILES:.c=.o)
INPUT_FILE = infile

#Library path
LIBDIR = ./libft
LIBFT = $(LIBDIR)/libft.a

#Bonus marker file
BONUS_FLAG = .bonus

#Default rule: compile all
all: $(LIBFT) $(NAME)

#Rule to create the program (removes bonus flag if exists)
$(NAME): $(OBJECTS) $(LIBFT)
	@rm -f $(BONUS_FLAG)
	$(CC) $(CFLAGS) $(OBJECTS) -L$(LIBDIR) -lft -o $(NAME)

#Rule to create the needed library
$(LIBFT):
	make -C $(LIBDIR) all

$(INPUT_FILE):
	touch $(INPUT_FILE)

#Auto compile .c into .o
%.o: %.c pipex.h $(LIBDIR)/libft.h
	$(CC) $(CFLAGS) -c $< -o $@

#Rule to clean all the created files
clean:
	$(MAKE) -C $(LIBDIR) clean
	rm -f $(OBJECTS) $(B_OBJECTS)

#Rule to clean all, including the library
fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	rm -f $(NAME) $(BONUS_FLAG)

#Rule to recompile all
re: fclean all

#Bonus rule - only recompiles if needed
bonus: $(LIBFT) $(BONUS_FLAG)

$(BONUS_FLAG): $(B_OBJECTS) $(LIBFT)
	$(CC) $(CFLAGS) $(B_OBJECTS) -L$(LIBDIR) -lft -o $(NAME)
	@touch $(BONUS_FLAG)

#Indicates that the next rules are not files
.PHONY: all clean fclean re bonus