#include "graph.hh"

void
Graph::print_helper (NodeIds &nodes, unsigned int *data)
{
    NodeIdIter node = nodes.begin (), node_end = nodes.end ();
    unsigned int tmp = data[*node];
    cout << "{";
    if (tmp == ((unsigned int) -1))
        cout << 'n';
    else
        cout << tmp;
    for (++node; node != node_end; ++node)
    {
        tmp = data[*node];
        cout << ",";
        if (tmp == (unsigned int) -1)
            cout << 'n';
        else
            cout << tmp;
    }
    cout << "}";
}

void
Graph::search_print (NodeIds &nodes, queue <NodeId> nexts, unsigned int time, NodeId node, map <NodeId, NodeColor> &colors, NodeId *ances, NodeId *deltas, NodeId *starts, NodeId *ends)
{
    cout << " ";
    if (time == (unsigned int) -1)
        cout << "-";
    else
        cout << time;
    cout << " | ";
    if (node == (NodeId) -1)
        cout << "-";
    else
        cout << node;
    cout << " | {";
    if (deltas != 0) /* deltas is null for depth-first, not for breadth-first */
    {
        if (!nexts.empty ())
        {
            cout << nexts.front ();
            nexts.pop ();
        }
        while (!nexts.empty ())
        {
            cout << "," << nexts.front ();
            nexts.pop ();
        }
        cout << "} | {";
    }
    NodeIdIter nod = nodes.begin (), nod_end = nodes.end ();
    cout << static_cast <char> (colors[*nod]);
    for (++nod; nod != nod_end; ++nod)
        cout << "," << static_cast <char> (colors[*nod]);
    cout << "} | ";
    print_helper (nodes, ances);
    cout << " | ";
    if (deltas != 0)
    {
        print_helper (nodes, deltas);
        cout << " | ";
    }
    print_helper (nodes, starts);
    cout << " | ";
    print_helper (nodes, ends);
    cout << endl;
}

void
Graph::visit (NodeIds &nodes, NodeId node, unsigned int &time, map <NodeId, NodeColor> &colors, NodeId **ances, NodeId **starts, NodeId **ends, bool print)
{
    colors[node] = GREY;
    (*starts)[node] = time;

    if (print)
        depth_first_search_print (nodes, time, node, colors, *ances, *starts, *ends);

    ++time;
    NodeIds successors = this->list_successors (node);
    for (NodeIdIter i = successors.begin (), i_end = successors.end (); i != i_end; ++i)
    {
        if (colors[*i] == WHITE)
        {
            (*ances)[*i] = node;
            visit (nodes, *i, time, colors, ances, starts, ends, print);
        }
    }
    colors[node] = BLACK;
    (*ends)[node] = time;

    if (print)
        depth_first_search_print (nodes, time, node, colors, *ances, *starts, *ends);

    ++time;
}

void
Graph::depth_first_search (NodeId start, bool print)
{
    NodeIds nodes = this->list_nodes ();
    unsigned int count = nodes.size ();
    map <NodeId, NodeColor> colors;
    NodeId *ances = new NodeId[count];
    unsigned int *starts = new unsigned int[count];
    unsigned int *ends = new unsigned int[count];
    for (NodeIdIter i = nodes.begin (), i_end = nodes.end (); i != i_end; ++i)
    {
        ances[*i] = -1;
        starts[*i] = -1;
        ends[*i] = -1;
        colors[*i] = WHITE;
    }
    unsigned int time = 0;
    ances[start] = start;

    if (print)
        depth_first_search_print (nodes, -1, -1, colors, ances, starts, ends);

    visit (nodes, start, time, colors, &ances, &starts, &ends, print);

    delete[] (ances);
    delete[] (starts);
    delete[] (ends);
}

void
Graph::breadth_first_search (NodeId start, bool print)
{
    NodeIds nodes = this->list_nodes ();
    unsigned int count = nodes.size ();
    map <NodeId, NodeColor> colors;
    std::queue <NodeId> nexts;
    NodeId *ances = new NodeId[count];
    unsigned int *deltas = new unsigned int[count];
    unsigned int *starts = new unsigned int[count];
    unsigned int *ends = new unsigned int[count];
    for (NodeIdIter i = nodes.begin (), i_end = nodes.end (); i != i_end; ++i)
    {
        ances[*i] = -1;
        deltas[*i] = -1;
        starts[*i] = -1;
        ends[*i] = -1;
        colors[*i] = WHITE;
    }
    unsigned int time = 0;
    ances[start] = start;
    colors[start] = GREY;
    deltas[start] = 0;
    nexts.push (start);

    if (print)
        search_print (nodes, nexts, -1, -1, colors, ances, deltas, starts, ends);

    while (!nexts.empty ())
    {
        NodeId node = nexts.front ();
        nexts.pop ();
        starts[node] = time;
        NodeIds successors = this->list_successors (node);
        for (NodeIdIter i = successors.begin (), i_end = successors.end (); i != i_end; ++i)
        {
            if (colors[*i] == WHITE)
            {
                colors[*i] = GREY;
                ances[*i] = node;
                deltas[*i] = deltas[node] + 1;
                nexts.push (*i);
            }
        }

        if (print)
            search_print (nodes, nexts, time, node, colors, ances, deltas, starts, ends);

        ++time;
        colors[node] = BLACK;
        ends[node] = time;

        if (print)
            search_print (nodes, nexts, time, node, colors, ances, deltas, starts, ends);

        ++time;
    }

    delete[] (ances);
    delete[] (deltas);
    delete[] (starts);
    delete[] (ends);

}

bool
Graph::connex ()
{
    NodeIds nodes = this->list_nodes ();
    unsigned int unreachable_count = 0; /* 1 is acceptable, if there are no cycle, if some arcs are from here */
    for (NodeIdIter node = nodes.begin (), node_end = nodes.end (); node != node_end; ++node)
    {
        ArcIds arcs = this->list_arcs_to (*node);
        if (arcs.empty () && ++unreachable_count > 1)
            return false;
        bool reachable = false;
        for (ArcIdIter arc = arcs.begin (), arc_end = arcs.end (); arc != arc_end; ++arc)
        {
            if (this->arcs[*arc].first != *node)
            {
                reachable = true;
                break;
            }
        }
        if (!reachable)
        {
            if (++unreachable_count > 1)
                return false;
            arcs = this->list_arcs_from (*node);
            bool ok = false;
            for (ArcIdIter arc = arcs.begin (), arc_end = arcs.end (); arc != arc_end; ++arc)
            {
                if (this->arcs[*arc].second != *node)
                {
                    ok = true;
                    break;
                }
            }
            if (!ok)
                return false;
        }
    }
    return true;
}
