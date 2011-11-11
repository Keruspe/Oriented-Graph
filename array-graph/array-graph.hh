#ifndef __ARRAY_GRAPH_HH__
#define __ARRAY_GRAPH_HH__

#include "graph.hh"

#include <vector>

using std::vector;

typedef vector <ArcIds> EndNodes;
typedef vector <EndNodes> StartNodes;
typedef EndNodes::iterator EndNodeIter;
typedef StartNodes::iterator StartNodeIter;

class ArrayGraph : public Graph
{
private:
    /* To always get node X at index X (no deletion) */
    map <NodeId, bool> node_exists;
    /* Start node, end node, arcs */
    StartNodes matrix;
    unsigned int _nodes_count;
    unsigned int _arcs_count;
public:
    ArrayGraph ();
    ~ArrayGraph ();
    NodeId add_node ();
    void remove_node (NodeId id);
    ArcId add_arc (NodeId from, NodeId to);
    void remove_arc (ArcId id);
    NodeIds list_successors (NodeId id);
    NodeIds list_ancestors (NodeId id);
    NodeIds list_nodes ();
    ArcIds list_arcs_from (NodeId id);
    ArcIds list_arcs_to (NodeId id);
    unsigned int nodes_count () { return this->_nodes_count; }
    unsigned int arcs_count () { return this->_arcs_count; }
    friend ostream &operator<< (ostream &os, ArrayGraph &graph);
};

#endif /*__ARRAY_GRAPH_HH__*/
