# Мэйкфайл
# P.S. Драконы

NAME = ircserv

SRC =		srcs/bot/image/image_utils.cpp \
			srcs/bot/image/image.cpp \
			srcs/bot/bot_utils.cpp \
			srcs/bot/bot.cpp \
			srcs/handle/messegeHandler/additions.cpp \
			srcs/handle/users/user.cpp \
			srcs/handle/handle.cpp \
			srcs/listen/listen.cpp \
			srcs/listen/ping.cpp \
			srcs/listen/support.cpp \
			srcs/general.cpp \
			srcs/general.cpp \
			srcs/ircserv.cpp




HEADER =	srcs/handle/general/answers.hpp \
			srcs/handle/messegeHandler/additions.hpp \
			srcs/handle/messegeHandler/messegeHandler.hpp \
			srcs/handle/users/channel.hpp \
			srcs/handle/users/user.hpp \
			srcs/listen/listen.hpp \
			srcs/listen/ping.hpp \
			srcs/listen/support.hpp \
			srcs/general.hpp \
			srcs/bot/bot.hpp \
			srcs/bot/image/image.hpp

FLAGS	=   -Wall -Wextra -Werror -std=c++98

OBJ		=	$(patsubst %.cpp, %.o, $(SRC))

all		:	$(NAME)

$(NAME):	$(SRC) $(HEADER)
	clang++ $(FLAGS) -c $(SRC)
	clang++ $(FLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)
	rm -rf *.hpp.gch

fclean:	clean
	rm -rf $(NAME)

re:	fclean	all
