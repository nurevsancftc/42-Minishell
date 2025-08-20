NAME = minishell

SRCS =	src/main.c \
		src/main_loop.c \
		src/env.c \
		src/expander.c \
		src/expander_utils.c \
		src/expander_utils2.c \
		src/signals.c \
		builtins/cd.c \
		builtins/echo.c \
		builtins/env.c \
		builtins/export.c \
		builtins/export_utils.c \
		builtins/unset.c \
		builtins/utils.c \
		builtins/utils_2.c \
		builtins/exit.c \
		builtins/pwd.c \
		builtins/dot.c \
		executor/executor.c \
		executor/utils.c \
		executor/utils_2.c \
		executor/external_command.c \
		executor/pipeline_utils.c \
		executor/single_command.c \
		executor/pipeline.c \
		executor/commands.c \
		executor/heredocs.c \
		executor/heredoc_child.c \
		executor/heredoc_utils.c \
		executor/redirections.c \
		parser/lexer.c \
		parser/parser.c \
		parser/quotes.c \
		parser/redirections.c \
		parser/lexer_utils.c \
		parser/lexer_utils2.c \
		parser/parser_utils.c \
		parser/parser_utils2.c \

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