NAME        =   smartboi

SRCS		=	src/main.cpp \
					src/SmartBoi/SmartBoi.cpp \
					src/parsing/parse_API_response.cpp \
					src/parsing/parseRequests.cpp \
					src/utils/utils.cpp \
					src/SmartBoi/Weather_API.cpp

CXX         =   c++
FLAGS       =   -Wall -Wextra -Werror -std=c++98
RM          =   rm -rf

OBJS_DIR    =   build/
OBJS        =   $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(FLAGS) -c $< -o $@
	@echo "\033[92mCompiled: $<\033[0m"

clean:
	@$(RM) $(OBJS_DIR)
	@echo "\033[93mObject files and build directory cleaned.\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[93mExecutable cleaned.\033[0m"

re: fclean all

.PHONY: all clean fclean re