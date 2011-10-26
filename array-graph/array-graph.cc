#include "array-graph.hh"

ArrayGraph::ArrayGraph () :
    nodes_count (0),
    arcs_count (0)
{
}

ArrayGraph::~ArrayGraph ()
{
}

unsigned int
ArrayGraph::add_node ()
{
    unsigned int id = get_next_node_id ();
    node_exists[id] = true;
    ++nodes_count;
    matrice.push_back(vector < vector <unsigned int> >(nodes_count));
    return id;
}

void
ArrayGraph::delete_node (unsigned int id)
{
    node_exists[id] = false;
    --nodes_count;
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
