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

NodeId
ArrayGraph::add_node ()
{
    NodeId id = get_new_node_id ();
    node_exists[id] = true;
    ++_nodes_count;
    matrice.push_back (StartNode (_nodes_count - 1));
    for (StartNodeIter i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i)
        i->push_back (ArcIds ());
    return id;
}

void
ArrayGraph::remove_node (NodeId id)
{
    if (!node_exists[id])
        return;
    node_exists[id] = false;
    --_nodes_count;
    StartNode &node = matrice[id];
    for (EndNodeIter i = node.begin (), i_end = node.end (); i != i_end; ++i)
        _arcs_count -= i->size ();
    for (StartNodeIter i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i)
        _arcs_count -= (*i)[id].size ();
}

ArcId
ArrayGraph::add_arc (NodeId from, NodeId to)
{
    ArcId id = get_new_arc_id ();
    ++_arcs_count;
    matrice[from][to].insert (id);
    arcs[id] = std::make_pair <NodeId, NodeId> (from, to);
    return id;
}

void
ArrayGraph::remove_arc (ArcId id)
{
    pair <NodeId, NodeId> &nodes = arcs[id];
    ArcIds &tmp = matrice[nodes.first][nodes.second];
    for (ArcIdIter i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
    {
        if (*i != id)
            continue;
        tmp.erase (i);
        --_arcs_count;
        break;
    }
}

NodeIds
ArrayGraph::list_successors (NodeId id)
{
    NodeIds successors;
    if (!node_exists[id])
        return successors;

    StartNode &node = matrice[id];
    NodeId tmp = 0;
    for (EndNodeIter i = node.begin (), i_end = node.end (); i != i_end; ++i, ++tmp)
    {
        if (!node_exists[tmp])
            continue;
        if (!i->empty ())
            successors.insert (tmp);
    }
    return successors;
}

NodeIds
ArrayGraph::list_ancestors (NodeId id)
{
    NodeIds ancestors;
    if (!node_exists[id])
        return ancestors;

    NodeId tmp = 0;
    for (StartNodeIter i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i, ++tmp)
    {
        if (!node_exists[tmp])
            continue;
        if (!(*i)[id].empty ())
            ancestors.insert (tmp);
    }
    return ancestors;
}

NodeIds
ArrayGraph::list_nodes ()
{
    NodeIds nodes;
    for (NodeId i = 0; i < next_node_id (); ++i)
    {
        if (node_exists[i])
            nodes.insert (i);
    }
    return nodes;
}

ArcIds
ArrayGraph::list_arcs_from (NodeId id)
{
    ArcIds arcs_from;
    if (!node_exists[id])
        return arcs_from;

    StartNode &node = matrice[id];
    NodeId tmp = 0;
    for (EndNodeIter i = node.begin (), i_end = node.end (); i != i_end; ++i, ++tmp)
    {
        if (!node_exists[tmp])
            continue;
        for (ArcIdIter j = i->begin (), j_end = i->end (); j != j_end; ++j)
            arcs_from.insert (*j);
    }
    return arcs_from;
}

ArcIds
ArrayGraph::list_arcs_to (NodeId id)
{
    ArcIds arcs_to;
    if (!node_exists[id])
        return arcs_to;

    NodeId tmp = 0;
    for (StartNodeIter i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i, ++tmp)
    {
        if (!node_exists[tmp])
            continue;
        ArcIds &tmp = (*i)[id];
        for (ArcIdIter j = tmp.begin (), j_end = tmp.end (); j != j_end; ++j)
            arcs_to.insert (*j);
    }
    return arcs_to;
}

ArcIds
ArrayGraph::list_arcs ()
{
    ArcIds arc_ids; 
    for (StartNodeIter i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i)
    {
        for (EndNodeIter j = i->begin (), j_end = i->end (); j != j_end; ++j)
        {
            for (ArcIdIter k = j->begin (), k_end = j->end (); k != k_end; ++k)
                arc_ids.insert (*k);
        }
    }
    return arc_ids;
}

static void
print_helper (NodeIds &nodes, NodeId *others)
{
    NodeIdIter node = nodes.begin ();
    unsigned int tmp = others[*node];
    cout << "{";
    if (tmp == ((unsigned int) -1))
        cout << 'n';
    else
        cout << tmp;
    ++node;
    for (NodeIdIter node_end = nodes.end (); node != node_end; ++node)
    {
        tmp = others[*node];
        cout << ",";
        if (tmp == ((unsigned int) -1))
            cout << 'n';
        else
            cout << tmp;
    }
    cout << "}";
}


void
ArrayGraph::visit (NodeId node, unsigned int &time, map <NodeId, Color> &colors, NodeId **ances, NodeId **starts, NodeId **ends)
{
    colors[node] = GREY;
    (*starts)[node] = time;

    // Print stuff
    cout << " " << time << " | " << node << " | {";
    NodeIds nodes = this->list_nodes ();
    NodeIdIter nod = nodes.begin ();
    cout << static_cast <char> (colors[*nod]);
    ++nod;
    for (NodeIdIter nod_end = nodes.end (); nod != nod_end; ++nod)
        cout << "," << static_cast <char> (colors[*nod]);
    cout << "} | ";
    print_helper (nodes, *ances);
    cout << " | ";
    print_helper (nodes, *starts);
    cout << " | ";
    print_helper (nodes, *ends);
    cout << endl;
    // End of print stuff

    ++time;
    NodeIds successors = this->list_successors (node);
    for (NodeIdIter i = successors.begin (), i_end = successors.end (); i != i_end; ++i)
    {
        if (colors[*i] == WHITE)
        {
            (*ances)[*i] = node;
            visit (*i, time, colors, ances, starts, ends);
        }
    }
    colors[node] = BLACK;
    (*ends)[node] = time;

    // Print stuff
    cout << " " << time << " | " << node << " | {";
    nod = nodes.begin ();
    cout << static_cast <char> (colors[*nod]);
    ++nod;
    for (NodeIdIter nod_end = nodes.end (); nod != nod_end; ++nod)
        cout << "," << static_cast <char> (colors[*nod]);
    cout << "} | ";
    print_helper (nodes, *ances);
    cout << " | ";
    print_helper (nodes, *starts);
    cout << " | ";
    print_helper (nodes, *ends);
    cout << endl;
    // End of print stuff

    ++time;
}

void
ArrayGraph::depth_first_search (NodeId start)
{
    map <NodeId, Color> colors;
    unsigned int count = this->next_node_id ();
    NodeId *ances = new NodeId[count];
    NodeId *starts = new NodeId[count];
    NodeId *ends = new NodeId[count];
    unsigned int id = 0;
    for (StartNodeIter i = matrice.begin (), i_end = matrice.end (); i != i_end; ++i, ++id)
    {
        if (!node_exists[id])
            continue;
        ances[id] = -1;
        starts[id] = -1;
        ends[id] = -1;
        colors[id] = WHITE;
    }
    unsigned int time = 0;
    ances[start] = start;

    // Print stuff
    cout << " - | - | {";
    NodeIds nodes = this->list_nodes ();
    NodeIdIter node = nodes.begin ();
    cout << static_cast <char> (colors[*node]);
    ++node;
    for (NodeIdIter node_end = nodes.end (); node != node_end; ++node)
        cout << "," << static_cast <char> (colors[*node]);
    cout << "} | ";
    print_helper (nodes, ances);
    cout << " | ";
    print_helper (nodes, starts);
    cout << " | ";
    print_helper (nodes, ends);
    cout << endl;
    // End of print stuff

    visit (start, time, colors, &ances, &starts, &ends);
    delete[] (ances);
    delete[] (starts);
    delete[] (ends);
}

void
ArrayGraph::breadth_first_search (NodeId)
{
}

ostream &
operator<< (ostream &os, ArrayGraph &graph)
{
    os << " ";
    NodeIds nodes = graph.list_nodes ();
    for (NodeIdIter i = nodes.begin (), i_end = nodes.end (); i != i_end; ++i)
        os << "   " << *i;
    os << endl;
    for (NodeIdIter i = nodes.begin (), i_end = nodes.end (); i != i_end; ++i)
    {
        os << *i;
        // TODO: Make it pretty with big numbers or replace with true/false
        for (NodeIdIter j = nodes.begin (), j_end = nodes.end (); j != j_end; ++j)
            os << "   " << graph.matrice[*i][*j].size ();
        os << endl;
    }
    return os;
}
