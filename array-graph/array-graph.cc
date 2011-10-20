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
    unsigned int id = get_next_node_id ();
    unsigned int count = node_count ();
    nodes_alias[id] = count;
    matrice.push_back(vector < vector <unsigned int> >(count - 1));
    for (vector < vector < vector <unsigned int> > >::iterator i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i)
        i->push_back (vector <unsigned int> ());
    return id;
}

void
ArrayGraph::delete_node (unsigned int id)
{
    unsigned int alias, tmp = nodes_alias[id];
    for (vector < vector < vector <unsigned int> > >::iterator i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i, --tmp)
    {
        if (tmp == 0)
            matrice.erase (i);
        else
        {
            vector < vector <unsigned int> >::iterator target = i->begin ();
            for (unsigned int j = 0; j < alias; ++j)
                ++target;
            i->erase (target);
        }
    }
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
