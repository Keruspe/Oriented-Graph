#include "array-graph.hh"

ArrayGraph::ArrayGraph ()
{
}

ArrayGraph::~ArrayGraph ()
{
}

int
ArrayGraph::add_node ()
{
    return get_next_node_id (); 
}

void
ArrayGraph::delete_node (int id)
{
}

int
ArrayGraph::add_arc (int from, int to)
{
    return get_next_arc_id ();
}

void
ArrayGraph::remove_arc (int id)
{
}

set<int>
ArrayGraph::list_successors (int id)
{
    return set<int>();
}

set<int>
ArrayGraph::list_ancestors (int id)
{
    return set<int>();
}

set<int>
ArrayGraph::list_nodes ()
{
    return set<int>();
}

set<int>
ArrayGraph::list_arcs_from (int id)
{
    return set<int>();
}

set<int>
ArrayGraph::list_arcs_to (int id)
{
    return set<int>();
}

set<int>
ArrayGraph::list_arcs ()
{
    return set<int>();
}

int
ArrayGraph::node_count ()
{
    return 0;
}

int
ArrayGraph::arcs_count ()
{
    return 0;
}
