#ifndef __ARRAY_GRAPH_HH__
#define __ARRAY_GRAPH_HH__

#include "graph.hh"

#include <map>
#include <vector>

using std::map;
using std::pair;
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
    set <unsigned int> list_successors (unsigned int id);
    set <unsigned int> list_ancestors (unsigned int id);
    set <unsigned int> list_nodes ();
    set <unsigned int> list_arcs_from (unsigned int id);
    set <unsigned int> list_arcs_to (unsigned int id);
    set <unsigned int> list_arcs ();
    unsigned int nodes_count ();
    unsigned int arcs_count ();
private:
    /* To always get node X at index X (no deletion) */
    map <unsigned int, bool> node_exists;
    /* Start node, finish node, arcs */
    vector < vector < set <unsigned int> > > matrice;
    /* Arc id, node ids */
    map < unsigned int, pair <unsigned int, unsigned int> > arcs;
    unsigned int _nodes_count;
    unsigned int _arcs_count;
};

#endif /*__ARRAY_GRAPH_HH__*/
