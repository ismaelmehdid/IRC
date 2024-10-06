WEATHER_NAME	=	smartboi_weather
OPENAI_NAME		=	smartboi_openai

COMMON_SRCS		=	src/main.cpp \
					src/SmartBoi/SmartBoi.cpp \
					src/parsing/parse_API_response.cpp \
					src/parsing/parseRequests.cpp \
					src/utils/utils.cpp 

OPENAI_SRCS		=	src/SmartBoi/OpenAI_API.cpp
WEATHER_SRCS	= 	src/SmartBoi/Weather_API.cpp

CXX				=	c++
FLAGS			=	-Wall -Wextra -Werror -std=c++98

LDFLAGS_OPENAI	=	-lcurl

OPENAI_OBJS_DIR	=	build_openai/
WEATHER_OBJS_DIR =	build_weather/

OPENAI_OBJS		=	$(addprefix $(OPENAI_OBJS_DIR), $(COMMON_SRCS:.cpp=.o) $(OPENAI_SRCS:.cpp=.o))
WEATHER_OBJS	=	$(addprefix $(WEATHER_OBJS_DIR), $(COMMON_SRCS:.cpp=.o) $(WEATHER_SRCS:.cpp=.o))

all: weather

weather: CXXFLAGS += -D__WEATHER__
weather: $(WEATHER_NAME)

openai: CXXFLAGS += -D__OPENAI__
openai: LDFLAGS += $(LDFLAGS_OPENAI)
openai: $(OPENAI_NAME)

$(WEATHER_NAME): $(WEATHER_OBJS)
	@$(CXX) $(FLAGS) $(WEATHER_OBJS) -o $(WEATHER_NAME)

$(OPENAI_NAME): $(OPENAI_OBJS)
	@$(CXX) $(FLAGS) $(OPENAI_OBJS) -o $(OPENAI_NAME) $(LDFLAGS_OPENAI)

$(WEATHER_OBJS_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(FLAGS) $(CXXFLAGS) -c $< -o $@
	@echo "\033[92mCompiled: $<\033[0m"

$(OPENAI_OBJS_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(FLAGS) $(CXXFLAGS) -c $< -o $@
	@echo "\033[92mCompiled: $<\033[0m"

clean:
	@rm -rf $(OPENAI_OBJS_DIR) $(WEATHER_OBJS_DIR)
	@echo "\033[93mObject files and build directories cleaned.\033[0m"

fclean: clean
	@rm -f $(WEATHER_NAME) $(OPENAI_NAME)
	@echo "\033[93mExecutables cleaned.\033[0m"

re: fclean all

.PHONY: all weather openai clean fclean re
