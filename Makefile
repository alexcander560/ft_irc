# Мэйкфайл
# P.S. Драконы

NAME = ircserv

CPPFILES = */*.cpp */*/*.cpp */*/*/*.cpp

HPPFILES = */*.hpp */*/*.hpp */*/*/*.hpp

OBJFILES = *.o

all: $(NAME)

$(NAME): $(CPPFILES) $(HPPFILES)
	@clang++ -Wall -Wextra -Werror -c $(CPPFILES)
	@clang++ $(OBJFILES) -o $(NAME) -lcurl -std=c++98

clean:
	@rm -rf $(OBJFILES)
	@rm -rf *.hpp.gch

fclean:	clean
	@rm -rf $(NAME)

re:	fclean	all
