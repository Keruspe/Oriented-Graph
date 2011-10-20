all: oriented-graph

oriented-graph: oriented-graph.cc array-graph/array-graph.cc
	g++ -I. -I.. $^ -o $@

clean:
	rm -f oriented-graph

.PHONY: all clean
