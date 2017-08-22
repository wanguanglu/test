INCLUDEDIR= \
	-I ./ \
  -I /usr/include/eigen3

LIBDIR= \
	-lpthread -lm -lstdc++

GCC = g++
CPPFLAGS       = -g -finline-functions -Wall -Winline -pipe -Wno-deprecated -Wunused-variable -std=c++11

SRCWILD    = $(wildcard *.cpp)
OBJS       = $(patsubst %.cpp, %_at.o, $(SRCWILD))

BINS       = $(patsubst %.cpp, %.bin, $(SRCWILD))
FINAL_BINS = $(patsubst %.cpp, %, $(SRCWILD))

all : clean makebin



clean:
	rm -rf $(OBJS)
	rm -rf $(BINS)
	rm -rf $(FINAL_BINS)

%_at.o : %.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR)

%.bin  : %_at.o
	$(GCC) -g -o $@ $^ $(LIBDIR) $(INCLUDEDIR)

makebin: $(BINS)
	for bin in $(BINS);\
	do\
		newname=`echo $$bin | sed s/.bin//g`;\
		mv $$bin $$newname;\
	done
