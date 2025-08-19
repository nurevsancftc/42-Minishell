NAME = minishell

SRCS =	src/main.c \
		src/main_loop.c \
		src/env.c \
		src/expander.c \
		src/expander_utils.c \
		src/signals.c \
		builtins/cd.c \
		builtins/echo.c \
		builtins/env.c \
		builtins/export.c \
		builtins/unset.c \
		builtins/utils.c \
		builtins/exit.c \
		builtins/pwd.c \
		builtins/dot.c \
		executor/executor.c \
		executor/executor_utils.c \
		executor/executor_pipeline.c \
		executor/executor_commands.c \
		executor/heredocs.c \
		executor/redirections.c \
		parser/lexer.c \
		parser/parser.c \
		parser/quotes.c \
		parser/redirections.c \
		parser/lexer_utils.c \
		parser/parser_utils.c \


CC = cc
CFLAGS = -g -Iincludes

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a


all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) all
	@$(MAKE) -C $(LIBFT_DIR) bonus

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT) -lreadline
	@echo "minishell compiled successfully!"
	@echo "Run './$(NAME)' to start the shell."
	@echo "Type 'exit' to quit the shell."
	@echo "Have fun!"

clean:
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "Object files cleaned."

fclean:
	@rm -f $(OBJS) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "All files cleaned."

re: fclean all clean
	@echo "Recompiled successfully!"

.PHONY: all clean fclean re