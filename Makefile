# Adjust your compiler here
CXX = clang++ -std=c++14

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses -Wno-invalid-source-encoding

FLAGS = -DDEBUG -O0 -g $(WARN)

OPT = -DNDEBUG -Ofast -D__extern_always_inline="extern __always_inline" $(WARN)

INCLUDE = -I.

LIBS = -lc -lm


all: test compressor

test: test.C Huffman.H 
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

compressor: compressor.C Huffman.H
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

clean:
	$(RM) test compressor *~
