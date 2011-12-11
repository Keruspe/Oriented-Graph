#ifndef __ARRAY_DIGRAPH_HH__
#define __ARRAY_DIGRAPH_HH__

#include "digraph.hh"

#include <vector>

using std::vector;

typedef vector <ArcIds> EndNodes;
typedef vector <EndNodes> StartNodes;
typedef EndNodes::iterator EndNodeIter;
typedef StartNodes::iterator StartNodeIter;

class ArrayDiGraph : public DiGraph
{
private:
    /* To always get node X at index X (no real deletion) */
    map <NodeId, bool> node_exists;
    /* Start node, end node, arcs */
    StartNodes matrix;
    unsigned int _nodes_count;
    unsigned int _arcs_count;
public:
    ArrayDiGraph ();
    ~ArrayDiGraph ();
    NodeId add_node ();
    void remove_node (NodeId id);
    ArcId add_arc (NodeId from, NodeId to);
    void remove_arc (ArcId id);
    NodeIds list_successors (NodeId id);
    NodeIds list_ancestors (NodeId id);
    NodeIds list_nodes ();
    ArcIds list_arcs_from (NodeId id);
    ArcIds list_arcs_to (NodeId id);
    ArcIds list_arcs_from_to (NodeId from, NodeId to);
    unsigned int nodes_count () { return this->_nodes_count; }
    unsigned int arcs_count () { return this->_arcs_count; }
};

#endif /*__ARRAY_DIGRAPH_HH__*/
