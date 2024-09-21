NAME    = ircserv

SRCS    =	src/main.cpp \
            src/utils/ErrorMessages.cpp \
            src/utils/SignalHandling.cpp \
            src/parsing/ParseRequests.cpp \
            src/parsing/ArgCheck.cpp \
            src/client/Client.cpp \
            src/server/Server.cpp \
            src/server/PollEvent.cpp \
            src/server/Socket.cpp \
            src/server/ChannelHandling.cpp \
            src/server/ClientHandling.cpp \
            src/server/CommandExecution/cap.cpp \
            src/server/CommandExecution/Execution.cpp \
            src/server/CommandExecution/invite.cpp \
            src/server/CommandExecution/join.cpp \
            src/server/CommandExecution/kick.cpp \
            src/server/CommandExecution/mode.cpp \
            src/server/CommandExecution/nick.cpp \
            src/server/CommandExecution/part.cpp \
            src/server/CommandExecution/pass.cpp \
            src/server/CommandExecution/ping.cpp \
            src/server/CommandExecution/privmsg.cpp \
            src/server/CommandExecution/quit.cpp \
			src/server/CommandExecution/serverMessages.cpp \
            src/server/CommandExecution/topic.cpp \
            src/server/CommandExecution/user.cpp \
            src/channel/Channel.cpp \
            src/exception/ServerBindException.cpp \
            src/exception/ServerListenException.cpp \
            src/exception/ServerCreationException.cpp \
            src/exception/PollException.cpp

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    OS_FLAGS = -D__linux__
else ifeq ($(UNAME_S), Darwin)
    OS_FLAGS = -D__APPLE__
endif

CC      = c++
FLAGS   = -Wall -Wextra -Werror -std=c++98 $(OS_FLAGS)

RM      = rm -rf

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
