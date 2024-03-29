#ifndef __DIGRAPH_HH__
#define __DIGRAPH_HH__

#include <iostream>
#include <list>
#include <map>
#include <queue>

using std::cout;
using std::endl;
using std::ostream;
using std::list;
using std::map;
using std::pair;
using std::queue;
using std::string;

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

class DiGraph
{
public:
    DiGraph () : _next_arc_id (0), _next_node_id (0) {}
    virtual ~DiGraph () {}
    virtual NodeId add_node () = 0;
    virtual void remove_node (NodeId id) = 0;
    virtual ArcId add_arc (NodeId from, NodeId to) = 0;
    virtual void remove_arc (ArcId id) = 0;
    virtual NodeIds list_successors (NodeId id) = 0;
    virtual NodeIds list_ancestors (NodeId id) = 0;
    virtual NodeIds list_nodes () = 0;
    virtual ArcIds list_arcs_from (NodeId id) = 0;
    virtual ArcIds list_arcs_to (NodeId id) = 0;
    virtual ArcIds list_arcs_from_to (NodeId from, NodeId to) = 0;
    virtual unsigned int nodes_count () = 0;
    virtual unsigned int arcs_count () = 0;
    void depth_first_search (NodeId start, bool print);
    void breadth_first_search (NodeId start, bool print);
    bool connex ();
    bool acyclic ();
    bool simple ();
    pair <NodeId, NodeId> get_arc_details (ArcId id);
    string as_matrix ();
    string as_list ();
private:
    ArcId _next_arc_id;
    NodeId _next_node_id;
    /* For depth_first_search */
    void visit (NodeIds &nodes, NodeId id, unsigned int &time, map <NodeId, NodeColor> &colors, NodeId *ancestors, NodeId *starts, NodeId *ends, bool print, NodeIds &nodes_explored);
    /* for search_print */
    void print_helper (NodeIds &nodes, unsigned int *data);
    void search_print (NodeIds &nodes, queue <NodeId> nexts, unsigned int time, NodeId id, map <NodeId, NodeColor> &colors, NodeId *ancestors, NodeId *deltas, NodeId *starts, NodeId *ends);
    /* Wrapper */
    void depth_first_search_print (NodeIds &nodes, unsigned int time, NodeId id, map <NodeId, NodeColor> &colors, NodeId *ancestors, NodeId *starts, NodeId *ends)
        {
            this->search_print (nodes, queue <NodeId> (), time, id, colors, ancestors, 0, starts, ends);
        }
    /* Adapted from Dijkstra */
    bool path_exists_between (NodeIds &nodes, NodeId from, NodeId to);
    bool path_exists_to_self (NodeIds &nodes, NodeId id) { return this->path_exists_between (nodes, id, id); }
protected:
    ArcId get_new_arc_id (NodeId from, NodeId to);
    NodeId get_new_node_id () { return this->_next_node_id++; }
    /* Used in ArrayGraph */
    NodeId next_node_id () { return this->_next_node_id; }
    /* Arc id, node ids */
    map < ArcId, pair <NodeId, NodeId> > arcs;
};

#endif /*__DIGRAPH_HH__*/
