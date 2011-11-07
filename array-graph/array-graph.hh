#ifndef __ARRAY_GRAPH_HH__
#define __ARRAY_GRAPH_HH__

#include "graph.hh"

#include <iostream>
#include <map>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::ostream;
using std::pair;
using std::vector;

typedef vector <ArcIds> StartNode;
typedef vector <StartNode> StartNodes;
typedef StartNodes::iterator StartNodeIter;
typedef StartNode::iterator EndNodeIter;

typedef enum {
    WHITE = 'w',
    GREY = 'g',
    BLACK = 'b'
} Color;

class ArrayGraph : public Graph
{
private:
    /* To always get node X at index X (no deletion) */
    map <NodeId, bool> node_exists;
    /* Start node, end node, arcs */
    StartNodes matrice;
    /* Arc id, node ids */
    map < ArcId, pair <NodeId, NodeId> > arcs;
    unsigned int _nodes_count;
    unsigned int _arcs_count;
    void visit (NodeId node, unsigned int &time, map <NodeId, Color> &colors, NodeId **ances, NodeId **starts, NodeId **ends, NodeIds &nodes);
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
    ArcIds list_arcs ();
    unsigned int nodes_count () { return _nodes_count; }
    unsigned int arcs_count () { return _arcs_count; }
    void depth_first_search (NodeId start);
    void breadth_first_search (NodeId start);
    friend ostream &operator<< (ostream &os, ArrayGraph &graph);
};

#endif /*__ARRAY_GRAPH_HH__*/
