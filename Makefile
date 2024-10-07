NAME        = ft_malcolm

CFLAGS      = -Wall -Wextra -Werror -std=c99
CC          = gcc
MAKE        = make

LIBFT_DIR   = libft
HEADERS      = inc/ft_malcolm.h

INCLUDE     = -I inc -I $(LIBFT_DIR)

SRCS        = src/main.c src/utils.c
OBJS        = $(patsubst %.c,%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS) libft
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft

%.o: %.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

libft:
	cd $(LIBFT_DIR) && $(MAKE)

clean:
	rm -f $(OBJS)
	cd $(LIBFT_DIR) && $(MAKE) clean

fclean: clean
	rm -f $(NAME)
	cd $(LIBFT_DIR) && $(MAKE) fclean

re: fclean all

.PHONY: all clean fclean re libft
