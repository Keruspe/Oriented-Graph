all: oriented-graph

oriented-graph: oriented-graph.cc graph.cc array-graph/array-graph.cc ListGraph/ListGraph.cc
	g++ -Wall -Wextra -O3 -march=native -I. -I.. -g -ggdb3 $^ -o $@ -Wl,--as-needed -Wl,-O3

clean:
	rm -f oriented-graph

.PHONY: all clean
