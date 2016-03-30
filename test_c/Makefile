INCLUDEDIR= \
	-I ./

LIBDIR= \
	-lcrypto -lpthread -lm -lstdc++	

GCC = g++

SRCWILD    = $(wildcard *.cpp)
OBJS       = $(patsubst %.cpp, %_at.o, $(SRCWILD))

BINS       = $(patsubst %.cpp, %, $(SRCWILD))

all : clean $(BINS)
	rm -rf $(OBJS)

CPPFLAGS       = -g -finline-functions -Wall -Winline -pipe -Wno-deprecated -Wunused-variable

$(LIBS) : $(OBJS)
	ar rcv $@ $^

clean:
	rm -rf $(OBJS)
	rm -rf $(BINS)

%_at.o : %.cc
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR)

%	   : %_at.o
	$(GCC) -g -o $@ $^ $(LIBDIR) $(INCLUDEDIR)
