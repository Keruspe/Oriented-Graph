#ifndef __GRAPH_HH__
#define __GRAPH_HH__

#include <string>

using std::string;

class Graph
{
public:
    virtual void add_node (string &label) = 0;
    virtual void delete_node (string &label) = 0;
    virtual void add_arc (string &label, string &from, string &to) = 0;
    virtual void remove_arc (string &label) = 0;
    virtual void list_successors (string &label) = 0;
    virtual void list_ancestors (string &label) = 0;
    virtual void list_arcs_from (string &label) = 0;
    virtual void list_arcs_to (string &label) = 0;
    virtual void node_count () = 0;
    virtual void arcs_count () = 0;
};

#endif /*__GRAPH_HH__*/
