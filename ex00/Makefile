NAME        := btc
CXX         := c++
CXXFLAGS    := -Wall -Wextra -Werror -std=c++11

SRCS        := main.cpp BitcoinExchange.cpp
OBJS        := $(SRCS:.cpp=.o)
DEPS        := $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
