NAME    = smartboi

COMMON_SRCS =   src/main.cpp \
                src/SmartBoi/SmartBoi.cpp \
                src/parsing/parse_API_response.cpp \
                src/parsing/parseRequests.cpp \
                src/utils/utils.cpp \

OPENAI_SRCS =   src/SmartBoi/OpenAI_API.cpp
WEATHER_SRCS =  src/SmartBoi/Weather_API.cpp

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    OS_FLAGS = -D__linux__
else ifeq ($(UNAME_S), Darwin)
    OS_FLAGS = -D__APPLE__
endif

CC      = c++
FLAGS   = -Wall -Wextra -Werror -std=c++98 $(OS_FLAGS)
LDFLAGS =

RM      = rm -rf

OBJS_DIR = build/

COMMON_OBJS = $(addprefix $(OBJS_DIR), $(COMMON_SRCS:.cpp=.o))
OPENAI_OBJS = $(addprefix $(OBJS_DIR), $(OPENAI_SRCS:.cpp=.o))
WEATHER_OBJS = $(addprefix $(OBJS_DIR), $(WEATHER_SRCS:.cpp=.o))

all: weather

openai: LDFLAGS += -lcurl
openai: FLAGS += -D__OPENAI__
openai: $(COMMON_OBJS) $(OPENAI_OBJS)
	@$(CC) $(LDFLAGS) $(FLAGS) $(COMMON_OBJS) $(OPENAI_OBJS) -o $(NAME)
	@echo "\033[92mCompiled: $(NAME) with OpenAI support.\033[0m"

weather: FLAGS += -D__WEATHER__
weather: $(COMMON_OBJS) $(WEATHER_OBJS)
	@$(CC) $(LDFLAGS) $(FLAGS) $(COMMON_OBJS) $(WEATHER_OBJS) -o $(NAME)
	@echo "\033[92mCompiled: $(NAME) with Weather API support.\033[0m"

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

.PHONY: all clean fclean re openai weather
