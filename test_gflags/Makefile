NAME = hello-world

LIBS = -lgflags -lpthread

debug: all

run-debug: ./${NAME}

all: $(NAME).o
	 c++ -lstdc++ $(LIBS) -o $(NAME) $(NAME).o

compile: $(NAME).o

clean:
	find . -name '*.o' -exec rm -f {} ';'
	find . -name $(NAME) -exec rm -f {} ';'

$(NAME).o: $(NAME).cpp
	gcc -c -I. -o $(NAME).o $(NAME).cpp

.cpp.o:
	gcc -c -I. -o $@ $<
