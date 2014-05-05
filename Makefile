CXX ?= g++
CC ?= gcc
CFLAGS = -Wall -Wconversion -O3 -fPIC
CFLAGS_DEBUG = -Wall
INCPATHS = -I . -I ./include

all: clean train recom_feature_maker

train:
	$(CXX) -ggdb -o train src/train.cpp src/fm_rec_algorithm.cpp src/data.cpp src/util.cpp src/evaluator.cpp src/l1rlr_owlqn_algorithm.cpp src/auc_onepass_algorithm.cpp $(INCPATHS)

recom_feature_maker:
	$(CXX) -o recom_feature_maker src/recom_feature_maker.cpp src/util.cpp $(INCPATHS)

clean:
	rm -f *.o train recom_feature_maker
