#ifndef __GRAPH_HH__
#define __GRAPH_HH__

#include <list>

using std::list;

typedef unsigned int NodeId;
typedef list <NodeId> NodeIds;
typedef unsigned int ArcId;
typedef list <ArcId> ArcIds;
typedef NodeIds::iterator NodeIdIter;
typedef ArcIds::iterator ArcIdIter;

typedef const list <unsigned int> ConstIds;
typedef ConstIds::const_iterator ConstIdIter;

typedef enum {
    WHITE = 'w',
    GREY = 'g',
    BLACK = 'b'
} NodeColor;

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
    virtual ArcIds list_arcs_from (NodeId id) = 0;
    virtual ArcIds list_arcs_to (NodeId id) = 0;
    virtual unsigned int nodes_count () = 0;
    virtual unsigned int arcs_count () = 0;
    virtual void depth_first_search (NodeId start) = 0;
    virtual void breadth_first_search (NodeId start) = 0;
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
