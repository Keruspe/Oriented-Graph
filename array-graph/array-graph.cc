#include "array-graph.hh"

ArrayGraph::ArrayGraph () :
    Graph (),
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
    unsigned int id = get_new_node_id ();
    node_exists[id] = true;
    ++_nodes_count;
    matrice.push_back (vector < set <unsigned int> >(_nodes_count - 1));
    for (vector < vector < set <unsigned int> > >::iterator i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i)
        i->push_back (set <unsigned int> ());
    return id;
}

void
ArrayGraph::delete_node (unsigned int id)
{
    if (!node_exists[id])
        return;
    node_exists[id] = false;
    --_nodes_count;
    vector < set <unsigned int> > &node = matrice[id];
    for (vector < set <unsigned int> >::iterator i = node.begin (), i_end = node.end (); i != i_end; ++i)
        _arcs_count -= i->size ();
    for (vector < vector < set <unsigned int> > >::iterator i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i)
        _arcs_count -= (*i)[id].size ();
}

unsigned int
ArrayGraph::add_arc (unsigned int from, unsigned int to)
{
    unsigned int id = get_new_arc_id ();
    ++_arcs_count;
    matrice[from][to].insert (id);
    arcs[id] = std::make_pair <unsigned int> (from, to);
    return id;
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

    vector < set <unsigned int> > &node = matrice[id];
    unsigned int tmp = 0;
    for (vector < set <unsigned int> >::iterator i = node.begin (), i_end = node.end (); i != i_end; ++i, ++tmp)
    {
        if (!node_exists[tmp])
            continue;
        for (set <unsigned int>::iterator j = i->begin (), j_end = i->end (); j != j_end; ++j)
            arcs_from.insert (*j);
    }
    return arcs_from;
}

set <unsigned int>
ArrayGraph::list_arcs_to (unsigned int id)
{
    set <unsigned int> arcs_to;
    if (!node_exists[id])
        return arcs_to;

    unsigned int tmp = 0;
    for (vector < vector < set <unsigned int> > >::iterator i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i, ++tmp)
    {
        if (!node_exists[tmp])
            continue;
        set <unsigned int> &tmp = (*i)[id];
        for (set <unsigned int>::iterator j = tmp.begin (), j_end = tmp.end (); j != j_end; ++j)
            arcs_to.insert (*j);
    }
    return arcs_to;
}

set <unsigned int>
ArrayGraph::list_arcs ()
{
    set <unsigned int> arc_ids; 
    for (vector < vector < set <unsigned int> > >::iterator i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i)
    {
        for (vector < set <unsigned int> >::iterator j = i->begin (), j_end = i->end (); j != j_end; ++j)
        {
            for (set <unsigned int>::iterator k = j->begin (), k_end = j->end (); k != k_end; ++k)
                arc_ids.insert (*k);
        }
    }
    return arc_ids;
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
