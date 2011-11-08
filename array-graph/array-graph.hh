#ifndef __ARRAY_GRAPH_HH__
#define __ARRAY_GRAPH_HH__

#include "graph.hh"

#include <iostream>
#include <map>
#include <queue>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::ostream;
using std::pair;
using std::queue;
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
    StartNodes matrice;
    /* Arc id, node ids */
    map < ArcId, pair <NodeId, NodeId> > arcs;
    unsigned int _nodes_count;
    unsigned int _arcs_count;
    /* For depth_first_search */
    void visit (NodeId node, unsigned int &time, map <NodeId, NodeColor> &colors, NodeId **ances, NodeId **starts, NodeId **ends);
    /* for search_print */
    void print_helper (unsigned int *data);
    void search_print (queue <NodeId> nexts, unsigned int time, NodeId node, map <NodeId, NodeColor> &colors, NodeId *ances, NodeId *deltas, NodeId *starts, NodeId *ends);
    /* Wrapper */
    void depth_first_search_print (unsigned int time, NodeId node, map <NodeId, NodeColor> &colors, NodeId *ances, NodeId *starts, NodeId *ends)
        {
            search_print (queue <NodeId> (), time, node, colors, ances, 0, starts, ends);
        }

public:
    ArrayGraph ();
    ~ArrayGraph ();
    NodeId add_node ();
    void remove_node (NodeId id);
    ArcId add_arc (NodeId from, NodeId to);
    void remove_arc (ArcId id);
    NodeIds list_successors (NodeId id);
    NodeIds list_ancestors (NodeId id);
    ArcIds list_arcs_from (NodeId id);
    ArcIds list_arcs_to (NodeId id);
    unsigned int nodes_count () { return _nodes_count; }
    unsigned int arcs_count () { return _arcs_count; }
    void depth_first_search (NodeId start);
    void breadth_first_search (NodeId start);
    friend ostream &operator<< (ostream &os, ArrayGraph &graph);
};

#endif /*__ARRAY_GRAPH_HH__*/
