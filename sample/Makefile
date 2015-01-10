PROGRAM = blif_stat
CXX = g++
CXX_OPTS = -std=c++11 -O2
LIBS = -lblif

all: $(PROGRAM)

$(PROGRAM): blif_stat.cpp
	$(CXX) $(CXX_OPTS) $< -o $@ $(LIBS)
