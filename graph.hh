#ifndef __GRAPH_HH__
#define __GRAPH_HH__

#include <set>

using std::set;

class Graph
{
public:
    Graph () : current_node_id (0), current_arc_id (0) {}
    virtual unsigned int add_node () = 0;
    virtual void delete_node (unsigned int id) = 0;
    virtual unsigned int add_arc (unsigned int from, unsigned int to) = 0;
    virtual void remove_arc (unsigned int id) = 0;
    virtual set <unsigned int> list_successors (unsigned int id) = 0;
    virtual set <unsigned int> list_ancestors (unsigned int id) = 0;
    virtual set <unsigned int> list_nodes () = 0;
    virtual set <unsigned int> list_arcs_from (unsigned int id) = 0;
    virtual set <unsigned int> list_arcs_to (unsigned int id) = 0;
    virtual set <unsigned int> list_arcs () = 0;
    virtual unsigned int nodes_count () = 0;
    virtual unsigned int arcs_count () = 0;
private:
    unsigned int current_node_id;
    unsigned int current_arc_id;
protected:
    unsigned int get_new_node_id () { return current_node_id++; }
    unsigned int get_new_arc_id ()  { return current_arc_id++;  }
};

#endif /*__GRAPH_HH__*/
