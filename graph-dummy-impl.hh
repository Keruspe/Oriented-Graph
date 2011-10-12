#ifndef __GRAPH_DUMMY_IMPL_HH__
#define __GRAPH_DUMMY_IMPL_HH__

#include "graph.hh"

class Graph2 : public Graph
{
public:
    Graph2 () {};
    ~Graph2 () {};
     void add_node (string &label){};
     void delete_node (string &label){};
     void add_arc (string &label, string &from, string &to){};
     void remove_arc (string &label){};
     void list_successors (string &label){};
     void list_ancestors (string &label){};
     void list_arcs_from (string &label){};
     void list_arcs_to (string &label){};
     void node_count (){};
     void arcs_count (){};
};

#endif /*__GRAPH_DUMMY_IMPL_HH__*/
