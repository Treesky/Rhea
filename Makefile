CXX ?= g++
CC ?= gcc
CFLAGS = -Wall -Wconversion -O3 -fPIC
INCPATHS = -I . -I ./include

all: clean train feature_maker

train:
	$(CXX) $(CFLAGS) -o train src/train.cpp src/algorithm.cpp src/data.cpp $(INCPATHS)

feature_maker:
	$(CXX) $(CFLAGS) -o feature_make src/feature_maker.cpp $(INCPATHS)

clean:
	rm -f *.o train
