#include "array-graph.hh"

ArrayGraph::ArrayGraph () :
    _nodes_count (0),
    _arcs_count (0)
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
    ++_nodes_count;
    matrice.push_back(vector < vector <unsigned int> >(_nodes_count));
    return id;
}

void
ArrayGraph::delete_node (unsigned int id)
{
    node_exists[id] = false;
    --_nodes_count;
}

unsigned int
ArrayGraph::add_arc (unsigned int from, unsigned int to)
{
    unsigned int id = get_next_arc_id ();
    ++_arcs_count;
    matrice[from][to].push_back (id);
    arcs[id] = std::make_pair <unsigned int> (from, to);
}

void
ArrayGraph::remove_arc (unsigned int id)
{
    pair <unsigned int, unsigned int> &nodes = arcs[id];
    vector <unsigned int> &tmp = matrice[nodes.first][nodes.second];
    for (vector <unsigned int>::iterator i = tmp.begin(), i_end = tmp.end(); i != i_end; ++i)
    {
        if (*i != id)
            continue;
        tmp.erase (i);
        --_arcs_count;
        break;
    }
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
