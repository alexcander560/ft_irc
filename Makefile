#Our beautiful make

NAME = ircserv

CPPFILES = *.cpp

HPPFILES = *.hpp

OBJFILES = *.o

all: $(NAME)

$(NAME):
	clang++ -c $(CPPFILES)
	clang++ $(OBJFILES) -o $(NAME)

clean:
	rm -rf $(OBJFILES)

fclean:	clean
	rm -rf $(NAME)

re:	fclean	all