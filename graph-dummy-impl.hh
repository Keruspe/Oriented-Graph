#ifndef __GRAPH_DUMMY_IMPL_HH__
#define __GRAPH_DUMMY_IMPL_HH__

#include "graph.hh"

class Graph2 : public Graph
{
public:
    Graph2 () {}
    ~Graph2 () {}
    int add_node () {}
    void delete_node (int id) {}
    int add_arc (int from, int to) {}
    void remove_arc (int id) {}
    set<int> list_successors (int id) {}
    set<int> list_ancestors (int id) {}
    set<int> list_arcs_from (int id) {}
    set<int> list_arcs_to (int id) {}
    int node_count () {}
    int arcs_count () {}
};

#endif /*__GRAPH_DUMMY_IMPL_HH__*/
