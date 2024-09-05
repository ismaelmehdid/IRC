NAME = ircserv

SRCS =	src/main.cpp \
		src/utils/error_messages.cpp \
        src/utils/cleanup_and_exit.cpp

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -rf

OBJS_DIR = build/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

all: $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)src/utils

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)src/%.o: src/%.cpp
	$(CC) $(FLAGS) -c $< -o $@
	@echo "\033[92mCompiled: $<\033[0m"

clean:
	$(RM) $(OBJS_DIR)
	@echo "\033[93mObject files and build directory cleaned.\033[0m"

fclean: clean
	$(RM) $(NAME)
	@echo "\033[93mExecutable cleaned.\033[0m"

re: fclean all