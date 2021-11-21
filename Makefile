# Мэйкфайл
# P.S. Драконы

NAME = ircserv

CPPFILES = */*.cpp */*/*.cpp */*/*/*.cpp

HPPFILES = */*.hpp */*/*.hpp */*/*/*.hpp

OBJFILES = *.o

all: $(NAME)

$(NAME): $(CPPFILES) $(HPPFILES)
	@clang++ -c $(CPPFILES)
	@clang++ $(OBJFILES) -o $(NAME) -lcurl -lcrypt

clean:
	@rm -rf $(OBJFILES)
	@rm -rf *.hpp.gch

fclean:	clean
	@rm -rf $(NAME)

re:	fclean	all
