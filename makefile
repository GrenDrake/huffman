CXXFLAGS=-Wall -g -std=c++11 -pedantic
OBJS=huffman.o huffman_test.o

huffman: $(OBJS)
	$(CXX) $(OBJS) -o huffman