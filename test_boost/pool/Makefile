LOG   = ../../../third/glog/
BOOST = ../../../third/boost_1_63_0/

INCLUDEDIR= \
	-I $(BOOST)/include\
	-I $(LOG)/include\
	-I ./

LIBDIR= \
	-L $(LOG)/lib -lglog\
	-L $(BOOST)/lib \
    -lboost_iostreams\
		-lboost_filesystem\
		-lboost_system\
		-lboost_thread\
		-lcrypto -lpthread -lm -lrt -lstdc++	

GCC = g++
CPPFLAGS       = -g -finline-functions -Wall -Winline -pipe -Wno-deprecated -Wunused-variable

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
