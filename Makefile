NAME = ircserv

SRC = parssing.cpp server.cpp Message.cpp client.cpp channel.cpp mode.cpp joinChannel.cpp privmsg.cpp TOPIC.cpp invite.cpp kick.cpp bot.cpp

HEADER = headerfile.hpp Message.hpp client.hpp channel.hpp

OBJ = $(SRC:.cpp=.o)

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

$(NAME): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
