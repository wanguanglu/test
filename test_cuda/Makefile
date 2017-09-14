INCLUDEDIR= \
	-I ./

LIBDIR= \
	-lpthread -lm -lstdc++	

NVCC = /usr/local/cuda/bin/nvcc

SRCWILD	= $(wildcard *.cu)

BINS	   = $(patsubst %.cu, %.bin, $(SRCWILD))
FINAL_BINS = $(patsubst %.cu, %, $(SRCWILD))

all : clean makebin

clean:
	rm -rf $(BINS)
	rm -rf $(FINAL_BINS)

%.bin : %.cu
	$(NVCC) -o $@ $^

makebin: $(BINS)
	for bin in $(BINS);\
	do\
		newname=`echo $$bin | sed s/.bin//g`;\
		mv $$bin $$newname;\
	done
