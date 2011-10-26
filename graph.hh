#ifndef __GRAPH_HH__
#define __GRAPH_HH__

#include <set>

using std::set;

typedef unsigned int NodeId;
typedef set <NodeId> NodeIds;
typedef unsigned int ArcId;
typedef set <ArcId> ArcIds;

class Graph
{
public:
    Graph () : current_node_id (0), current_arc_id (0) {}
    virtual NodeId add_node () = 0;
    virtual void delete_node (NodeId id) = 0;
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
    NodeId current_node_id;
    ArcId current_arc_id;
protected:
    NodeId get_new_node_id () { return current_node_id++; }
    ArcId get_new_arc_id ()  { return current_arc_id++;  }
};

#endif /*__GRAPH_HH__*/
