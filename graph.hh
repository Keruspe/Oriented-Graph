#ifndef __GRAPH_HH__
#define __GRAPH_HH__

#include <set>

using std::set;

class Graph
{
public:
    virtual int add_node () = 0;
    virtual void delete_node (int id) = 0;
    virtual int add_arc (int from, int to) = 0;
    virtual void remove_arc (int id) = 0;
    virtual set<int> list_successors (int id) = 0;
    virtual set<int> list_ancestors (int id) = 0;
    virtual set<int> list_nodes () = 0;
    virtual set<int> list_arcs_from (int id) = 0;
    virtual set<int> list_arcs_to (int id) = 0;
    virtual set<int> list_arcs () = 0;
    virtual int node_count () = 0;
    virtual int arcs_count () = 0;
private:
    int next_node_id;
    int next_arc_id;
protected:
    int get_next_node_id () { return next_node_id++; }
    int get_next_arc_id () { return next_arc_id++; }
};

#endif /*__GRAPH_HH__*/
