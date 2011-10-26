#ifndef __ARRAY_GRAPH_HH__
#define __ARRAY_GRAPH_HH__

#include "graph.hh"

#include <map>
#include <vector>

using std::map;
using std::pair;
using std::vector;

typedef vector <ArcIds> StartNode;
typedef vector <StartNode> StartNodes;
typedef StartNode::iterator EndNode;

class ArrayGraph : public Graph
{
public:
    ArrayGraph ();
    ~ArrayGraph ();
    NodeId add_node ();
    void delete_node (NodeId id);
    ArcId add_arc (NodeId from, NodeId to);
    void remove_arc (ArcId id);
    NodeIds list_successors (NodeId id);
    NodeIds list_ancestors (NodeId id);
    NodeIds list_nodes ();
    ArcIds list_arcs_from (NodeId id);
    ArcIds list_arcs_to (NodeId id);
    ArcIds list_arcs ();
    unsigned int nodes_count ();
    unsigned int arcs_count ();
private:
    /* To always get node X at index X (no deletion) */
    map <NodeId, bool> node_exists;
    /* Start node, end node, arcs */
    StartNodes matrice;
    /* Arc id, node ids */
    map < ArcId, pair <NodeId, NodeId> > arcs;
    unsigned int _nodes_count;
    unsigned int _arcs_count;
};

#endif /*__ARRAY_GRAPH_HH__*/
