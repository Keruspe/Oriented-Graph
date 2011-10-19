#ifndef __ARRAY_GRAPH_HH__
#define __ARRAY_GRAPH_HH__

#include "graph.hh"

class ArrayGraph : public Graph
{
public:
    ArrayGraph ();
    ~ArrayGraph ();
    int add_node ();
    void delete_node (int id);
    int add_arc (int from, int to);
    void remove_arc (int id);
    set<int> list_successors (int id);
    set<int> list_ancestors (int id);
    set<int> list_nodes ();
    set<int> list_arcs_from (int id);
    set<int> list_arcs_to (int id);
    set<int> list_arcs ();
    int node_count ();
    int arcs_count ();
};

#endif /*__ARRAY_GRAPH_HH__*/
