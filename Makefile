NAME = ircserv

SRCS =	src/main.cpp \
        src/utils/error_messages.cpp \
        src/utils/signal_handling.cpp \
        src/server/Server.cpp \
        src/server/PollEvent.cpp \
        src/server/Socket.cpp \
        src/client/Client.cpp \
		src/client/ARole.cpp \
        src/client/OperatorRole.cpp \
        src/client/RegularRole.cpp \
        src/channel/Channel.cpp \
        src/parsing/arguments_checking.cpp \
        src/parsing/commands.cpp \
        src/exception/ServerBindException.cpp \
        src/exception/ServerListenException.cpp \
        src/exception/ServerCreationException.cpp \
        src/exception/PollException.cpp

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -rf

OBJS_DIR = build/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "\033[92mCompiled: $<\033[0m"

clean:
	@$(RM) $(OBJS_DIR)
	@echo "\033[93mObject files and build directory cleaned.\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[93mExecutable cleaned.\033[0m"

re: fclean all