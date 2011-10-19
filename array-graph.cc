#include "array-graph.hh"

ArrayGraph::ArrayGraph ()
{
}

ArrayGraph::~ArrayGraph ()
{
}

unsigned int
ArrayGraph::add_node ()
{
    return get_next_node_id (); 
}

void
ArrayGraph::delete_node (unsigned int id)
{
}

unsigned int
ArrayGraph::add_arc (unsigned int from, unsigned int to)
{
    return get_next_arc_id ();
}

void
ArrayGraph::remove_arc (unsigned int id)
{
}

set<unsigned int>
ArrayGraph::list_successors (unsigned int id)
{
    return set<unsigned int>();
}

set<unsigned int>
ArrayGraph::list_ancestors (unsigned int id)
{
    return set<unsigned int>();
}

set<unsigned int>
ArrayGraph::list_nodes ()
{
    return set<unsigned int>();
}

set<unsigned int>
ArrayGraph::list_arcs_from (unsigned int id)
{
    return set<unsigned int>();
}

set<unsigned int>
ArrayGraph::list_arcs_to (unsigned int id)
{
    return set<unsigned int>();
}

set<unsigned int>
ArrayGraph::list_arcs ()
{
    return set<unsigned int>();
}

unsigned int
ArrayGraph::node_count ()
{
    return matrice.size();
}

unsigned int
ArrayGraph::arcs_count ()
{
    return 0;
}
