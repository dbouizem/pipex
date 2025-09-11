NAME = pipex
BONUS_NAME = pipex_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -Iincludes -Ilibft

SRCDIR = srcs
BONUSDIR = srcs_bonus
OBJDIR = obj
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = \
	$(SRCDIR)/path.c \
	$(SRCDIR)/errors.c \
	$(SRCDIR)/init.c \
	$(SRCDIR)/main.c \
	$(SRCDIR)/process.c \
	$(SRCDIR)/utils.c

BONUS_SRCS = \
	$(BONUSDIR)/main_bonus.c \
	$(BONUSDIR)/init_bonus.c \
	$(BONUSDIR)/file_bonus.c \
	$(BONUSDIR)/here_doc_bonus.c \
	$(BONUSDIR)/commands_bonus.c \
	$(BONUSDIR)/pipes_bonus.c \
	$(BONUSDIR)/process_bonus.c \
	$(BONUSDIR)/child_bonus.c \
	$(BONUSDIR)/utils_bonus.c \
	$(BONUSDIR)/errors_bonus.c \
	$(BONUSDIR)/cleanup_bonus.c

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
BONUS_OBJS = $(BONUS_SRCS:$(BONUSDIR)/%.c=$(OBJDIR)/%.o)

HEADERS = includes/pipex.h
BONUS_HEADERS = includes/pipex_bonus.h

all: $(NAME)

$(NAME): $(LIBFT) $(OBJDIR) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

bonus: $(LIBFT) $(OBJDIR) $(BONUS_OBJS)
	@$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(BONUS_NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(BONUSDIR)/%.c $(BONUS_HEADERS) | $(OBJDIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(BONUS_OBJDIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory

fclean: clean
	@rm -f $(NAME) $(BONUS_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory

re: fclean all

.PHONY: all bonus clean fclean re
