# Executable name
NAME = pipex

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Sources per mode
SRC_NORMAL = main.c pipex_utils.c pipex_extra.c
OBJ_NORMAL = $(SRC_NORMAL:.c=.o)

SRC_BONUS = main_bonus.c pipex_utils_bonus.c pipex_extra_bonus.c pipex_heredoc_bonus.c
OBJ_BONUS = $(SRC_BONUS:.c=.o)

# Library
LIBDIR = ./libft
LIBFT = $(LIBDIR)/libft.a

# Mode marker files
MARKER_NORMAL = .normal_build
MARKER_BONUS = .bonus_build

# Default rule (normal build)
all: $(NAME)

$(NAME): $(MARKER_NORMAL)

# Bonus rule (independent)
bonus: $(MARKER_BONUS)

# Build normal mode
$(MARKER_NORMAL): $(OBJ_NORMAL) $(LIBFT)
	@rm -f $(OBJ_BONUS) $(MARKER_BONUS)
	$(CC) $(CFLAGS) $(OBJ_NORMAL) -L$(LIBDIR) -lft -o $(NAME)
	@touch $(MARKER_NORMAL)

# Build bonus mode
$(MARKER_BONUS): $(OBJ_BONUS) $(LIBFT)
	@rm -f $(OBJ_NORMAL) $(MARKER_NORMAL)
	$(CC) $(CFLAGS) $(OBJ_BONUS) -L$(LIBDIR) -lft -o $(NAME)
	@touch $(MARKER_BONUS)

# Compile libft
$(LIBFT):
	@$(MAKE) -C $(LIBDIR) all

# Pattern rule for object files
%.o: %.c pipex.h $(LIBDIR)/libft.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object and mode markers
clean:
	@$(MAKE) -C $(LIBDIR) clean
	rm -f $(OBJ_NORMAL) $(OBJ_BONUS)
	rm -f $(MARKER_NORMAL) $(MARKER_BONUS)

# Full clean
fclean: clean
	@$(MAKE) -C $(LIBDIR) fclean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re bonus
