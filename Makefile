all: oriented-graph

oriented-graph: oriented-graph.cc
	g++ $< -o $@

clean:
	rm -f oriented-graph

.PHONY: all clean
