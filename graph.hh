#ifndef __GRAPH_HH__
#define __GRAPH_HH__

#include <set>

using std::set;

typedef unsigned int NodeId;
typedef set <NodeId> NodeIds;
typedef unsigned int ArcId;
typedef set <ArcId> ArcIds;
typedef ArcIds::iterator ArcIdIter;

class Graph
{
public:
    Graph () : _next_arc_id (0), _next_node_id (0) {}
    virtual ~Graph () {}
    virtual NodeId add_node () = 0;
    virtual void remove_node (NodeId id) = 0;
    virtual ArcId add_arc (NodeId from, NodeId to) = 0;
    virtual void remove_arc (ArcId id) = 0;
    virtual NodeIds list_successors (NodeId id) = 0;
    virtual NodeIds list_ancestors (NodeId id) = 0;
    virtual NodeIds list_nodes () = 0;
    virtual ArcIds list_arcs_from (NodeId id) = 0;
    virtual ArcIds list_arcs_to (NodeId id) = 0;
    virtual ArcIds list_arcs () = 0;
    virtual unsigned int nodes_count () = 0;
    virtual unsigned int arcs_count () = 0;
private:
    ArcId _next_arc_id;
    NodeId _next_node_id;
protected:
    ArcId get_new_arc_id ()  { return _next_arc_id++;  }
    NodeId get_new_node_id () { return _next_node_id++; }
    /* Used in ArrayGraph */
    NodeId next_node_id () { return _next_node_id; }
};

#endif /*__GRAPH_HH__*/
