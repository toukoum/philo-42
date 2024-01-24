NAME := philo

CC := cc 
CFLAGS := -Wall -Werror -Wextra -g -pthread

INCLUDE := -I include
SRCS := $(wildcard src/*.c)
OBJS :=  $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $(NAME) $(INCLUDE)

%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ 

re: fclean all

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

t:
	@./philo

	
.PHONY: all re clean fclean