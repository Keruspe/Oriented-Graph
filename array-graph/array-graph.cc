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
    matrice.push_back(vector < set <unsigned int> >(_nodes_count));
    return id;
}

void
ArrayGraph::delete_node (unsigned int id)
{
    if (!node_exists[id])
        return;
    node_exists[id] = false;
    --_nodes_count;
}

unsigned int
ArrayGraph::add_arc (unsigned int from, unsigned int to)
{
    unsigned int id = get_next_arc_id ();
    ++_arcs_count;
    matrice[from][to].insert (id);
    arcs[id] = std::make_pair <unsigned int> (from, to);
}

void
ArrayGraph::remove_arc (unsigned int id)
{
    pair <unsigned int, unsigned int> &nodes = arcs[id];
    set <unsigned int> &tmp = matrice[nodes.first][nodes.second];
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
    {
        if (*i != id)
            continue;
        tmp.erase (i);
        --_arcs_count;
        break;
    }
}

set <unsigned int>
ArrayGraph::list_successors (unsigned int id)
{
    set <unsigned int> successors;
    if (!node_exists[id])
        return successors;

    vector < set <unsigned int> > &node = matrice[id];
    unsigned int tmp = 0;
    for (vector < set <unsigned int> >::iterator i = node.begin (), i_end = node.end (); i != i_end; ++i, ++tmp)
    {
        if (!node_exists[tmp])
            continue;
        if (!i->empty ())
            successors.insert (tmp);
    }
    return successors;
}

set <unsigned int>
ArrayGraph::list_ancestors (unsigned int id)
{
    set <unsigned int> ancestors;
    if (!node_exists[id])
        return ancestors;

    unsigned int tmp = 0;
    for (vector < vector < set <unsigned int> > >::iterator i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i, ++tmp)
    {
        if (!node_exists[tmp])
            continue;
        if (!(*i)[id].empty ())
            ancestors.insert (tmp);
    }
    return ancestors;
}

set <unsigned int>
ArrayGraph::list_nodes ()
{
    set <unsigned int> nodes;
    for (unsigned int i = 0; i < matrice.size (); ++i)
    {
        if (node_exists[i])
            nodes.insert (i);
    }
    return nodes;
}

set <unsigned int>
ArrayGraph::list_arcs_from (unsigned int id)
{
    set <unsigned int> arcs_from;
    if (!node_exists[id])
        return arcs_from;

    vector < vector <unsigned int> > &node = matrice[id];
    unsigned int tmp = 0;
    for (vector < set <unsigned int> >::iterator i = node.begin (), i_end = node.end (); i != i_end; ++i, ++tmp)
    {
        if (!node_exists[tmp])
            continue;
        for (set <unsigned int>::iterator j = i->begin (), j_end = i->end (); j != j_end; ++i)
            arcs_from.insert (*j);
    }
    return arcs_from;
}

set <unsigned int>
ArrayGraph::list_arcs_to (unsigned int id)
{
    return set<unsigned int>();
}

set <unsigned int>
ArrayGraph::list_arcs ()
{
    // Do we really need this ?
    return set <unsigned int>();
}

unsigned int
ArrayGraph::nodes_count ()
{
    return _nodes_count;
}

unsigned int
ArrayGraph::arcs_count ()
{
    return _arcs_count;
}
