SRCS = ft_atoi.c \
       ft_atol.c \
       ft_bzero.c \
       ft_calloc.c \
       ft_fprintf.c \
       ft_isalnum.c \
       ft_isalpha.c \
       ft_isascii.c \
       ft_isdigit.c \
       ft_isprint.c \
       ft_itoa.c \
       ft_memchr.c \
       ft_memcmp.c \
       ft_memcpy.c \
       ft_memmove.c \
       ft_memset.c \
       ft_putchar_fd.c \
       ft_putstr_fd.c \
       ft_putendl_fd.c \
       ft_putnbr_fd.c \
       ft_split.c \
       ft_strchr.c \
       ft_strcmp.c \
       ft_strdup.c \
       ft_striteri.c \
       ft_strjoin.c \
       ft_strlcat.c \
       ft_strlcpy.c \
       ft_strlen.c \
       ft_strmapi.c \
       ft_strncmp.c \
       ft_strndup.c \
       ft_strnstr.c \
       ft_strrchr.c \
       ft_strtrim.c \
       ft_substr.c \
       ft_tolower.c \
       ft_toupper.c

OBJS = $(SRCS:.c=.o)

SRCBONUS =    ft_lstadd_back.c \
              ft_lstadd_front.c \
              ft_lstclear.c \
              ft_lstdelone.c \
              ft_lstiter.c \
              ft_lstlast.c \
              ft_lstmap.c \
              ft_lstnew.c \
              ft_lstsize.c

OBJSBONUS = $(SRCBONUS:.c=.o)

NAME = libft.a

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	ar rc $@ $^
	ranlib $@

%.o: %.c
	gcc -Wall -Wextra -Werror -c $< -o $@

.PHONY: bonus
bonus: $(OBJSBONUS)
	ar rc $(NAME) $^
	ranlib $(NAME)
	touch bonus

.PHONY: clean
clean:
	rm -f $(OBJS) $(OBJSBONUS)

.PHONY: fclean
fclean: clean
	rm -f $(NAME) bonus

.PHONY: re
re: fclean all
