CXX ?= g++
CC ?= gcc
CFLAGS = -Wall -Wconversion -O3 -fPIC
INCPATHS = -I . -I ./include

all: clean train recom_feature_maker

train:
	$(CXX) $(CFLAGS) -o train src/train.cpp src/algorithm.cpp src/data.cpp src/util.cpp $(INCPATHS)

recom_feature_maker:
	$(CXX) $(CFLAGS) -o recom_feature_maker src/recom_feature_maker.cpp src/util.cpp $(INCPATHS)

clean:
	rm -f *.o train recom_feature_maker