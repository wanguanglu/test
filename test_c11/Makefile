INCLUDEDIR= \
	-I ./

LIBDIR= \
	-lpthread -lm -lstdc++	

GCC = g++
CPPFLAGS       = -g -Wall -Winline -pipe -Wno-deprecated -Wunused-variable -std=c++11 -pthread

SRCWILD    = $(wildcard *.cc)
OBJS       = $(patsubst %.cc, %_at.o, $(SRCWILD))

BINS       = $(patsubst %.cc, %.bin, $(SRCWILD))
FINAL_BINS = $(patsubst %.cc, %, $(SRCWILD))

all : clean makebin



clean:
	rm -rf $(OBJS)
	rm -rf $(BINS)
	rm -rf $(FINAL_BINS)

%_at.o : %.cc
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR)

%.bin  : %_at.o
	$(GCC) -g -o $@ $^ $(LIBDIR) $(INCLUDEDIR)

makebin: $(BINS)
	for bin in $(BINS);\
	do\
		newname=`echo $$bin | sed s/.bin//g`;\
		mv $$bin $$newname;\
	done
