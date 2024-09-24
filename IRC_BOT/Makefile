NAME    = smartboi

SRCS    =	src/main.cpp \
			src/SmartBoi/SmartBoi.cpp \
			src/SmartBoi/OpenAI_API.cpp \
			src/SmartBoi/Weather_API.cpp \
			src/parsing/parse_API_response.cpp \
			src/parsing/parseRequests.cpp \
			src/utils/utils.cpp \

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    OS_FLAGS = -D__linux__
else ifeq ($(UNAME_S), Darwin)
    OS_FLAGS = -D__APPLE__
endif

CC      = c++
FLAGS   = -Wall -Wextra -Werror -std=c++98 $(OS_FLAGS)
LDFLAGS = -lcurl

RM      = rm -rf

OBJS_DIR = build/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(LDFLAGS) $(FLAGS) $(OBJS) -o $(NAME)

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
