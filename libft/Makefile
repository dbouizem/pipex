NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

OBJ_DIR = objs

SRCS =	ft_isalpha.c \
	ft_isdigit.c \
	ft_isalnum.c \
	ft_isascii.c \
	ft_isprint.c \
	ft_tolower.c \
	ft_toupper.c \
	ft_strlen.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_memset.c \
	ft_bzero.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_memchr.c \
	ft_calloc.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_strnstr.c \
	ft_strdup.c \
	ft_strncmp.c \
	ft_memcmp.c \
	ft_atoi.c \
	ft_striteri.c \
	ft_putstr_fd.c \
	ft_putchar_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_strtrim.c \
	ft_itoa.c \
	ft_strmapi.c \
	ft_split.c \
	ft_realloc.c

SRCS_BONUS =	ft_lstnew.c \
	ft_lstadd_front.c \
	ft_lstsize.c \
	ft_lstlast.c \
	ft_lstadd_back.c \
	ft_lstdelone.c \
	ft_lstclear.c \
	ft_lstiter.c \
	ft_lstmap.c

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
OBJS_BONUS = $(SRCS_BONUS:%.c=$(OBJ_DIR)/%.o)

all : $(NAME)

$(NAME) : $(OBJS)
	@ar rcs $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

bonus : $(OBJS) $(OBJS_BONUS)
	@ar rcs $(NAME) $(OBJS) $(OBJS_BONUS)

clean :
	@rm -rf $(OBJ_DIR) $(OBJS) $(OBJS_BONUS)

fclean : clean
	@rm -f $(NAME)

re : fclean all

rebonus : fclean bonus

.PHONY : clean fclean re all bonus rebonus
