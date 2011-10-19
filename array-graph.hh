#ifndef __ARRAY_GRAPH_HH__
#define __ARRAY_GRAPH_HH__

#include "graph.hh"

#include <vector>

using std::vector;

class ArrayGraph : public Graph
{
public:
    ArrayGraph ();
    ~ArrayGraph ();
    unsigned int add_node ();
    void delete_node (unsigned int id);
    unsigned int add_arc (unsigned int from, unsigned int to);
    void remove_arc (unsigned int id);
    set<unsigned int> list_successors (unsigned int id);
    set<unsigned int> list_ancestors (unsigned int id);
    set<unsigned int> list_nodes ();
    set<unsigned int> list_arcs_from (unsigned int id);
    set<unsigned int> list_arcs_to (unsigned int id);
    set<unsigned int> list_arcs ();
    unsigned int node_count ();
    unsigned int arcs_count ();
private:
    /* Start node, finish node, arcs */
    vector < vector < vector <unsigned int> > > matrice;
};

#endif /*__ARRAY_GRAPH_HH__*/
