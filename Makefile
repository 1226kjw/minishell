NAME 			= minishell

CC 				= gcc
CFLAGS 		= -Wall -Wextra #-Werror

LIBFT 		= libft.a
INCLUDE 	= includes/
SRCS_DIR 	= ./srcs/

RL				= -lreadline
RL_DIR		= /Users/$(USER)/.brew/opt/readline/
RL_LIB		= -L $(RL_DIR)lib
RL_INC		= -I $(RL_DIR)include

SRCS 			= $(wildcard $(SRCS_DIR)*.c)
OBJS 			= $(SRCS:.c=.o)
DEPS 			= $(SRCS:.c=.d)

#mine
RL_M1	= -L/usr/local/opt/readline/lib
RL_M2 = -I//usr/local/opt/readline/include

all : $(NAME)
$(NAME) : $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ -L. -lft $(RL) $(RL_LIB)

mine : $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L. -lft $(RL) $(RL_M1)

$(LIBFT) :
	@make all -C ./libft
	@cp ./libft/libft.h $(INCLUDE)
	@cp ./libft/libft.a ./

clean :
	@make clean -C ./libft
	@rm -f $(OBJS) $(DEPS)

fclean : clean
	@rm -f $(LIBFT) ./libft/libft.a
	@rm -f $(NAME)

re : fclean all

.c.o :
	@$(CC) $(CFLAGS) -MD -c -o $@ $< -I$(INCLUDE) $(RL_INC) $(RL_M2)

-include $(DEPS)

.PHONY: all clean fclean re
