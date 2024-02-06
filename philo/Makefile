NAME := philo

CC := cc 
CFLAGS := -Wall -Werror -Wextra -g -pthread

INCLUDE := -I include
SRCS := $(wildcard src/*.c)
OBJS :=  $(SRCS:.c=.o)

RED=\033[0;31m
ROSE=\033[0;95m
GREEN=\033[0;32m
NC=\033[0m
YELLOW=\033[0;33m
CYAN=\033[1;34m 

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Creating philosopher...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $(NAME) $(INCLUDE)
	@echo "$(CYAN) ====== DONE ! ======\n$(NC)"

%.o:%.c
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ 

re: fclean all

clean:
	@echo "\n$(RED)Cleaning project Philo...$(NC)"
	@rm -rf $(OBJS)

fclean: clean
	@echo "$(YELLOW)Deleted philo file...$(NC)"
	@rm -rf $(NAME)
	@echo "$(CYAN) ====== DONE ! ======\n$(NC)"

	
.PHONY: all re clean fclean