#include "graph.hh"

ArcId
Graph::get_new_arc_id (NodeId from, NodeId to)
{
    this->arcs[this->_next_arc_id] = std::make_pair <NodeId, NodeId> (from, to);
    return this->_next_arc_id++;
}

void
Graph::print_helper (NodeIds &nodes, unsigned int *data)
{
    NodeIdIter node = nodes.begin (), node_end = nodes.end ();
    cout << "{";
    if (node != node_end)
    {
        unsigned int value = data[*node];
        if (value == ((unsigned int) -1))
            cout << 'n';
        else
            cout << value;
        for (++node; node != node_end; ++node)
        {
            value = data[*node];
            cout << ",";
            if (value == (unsigned int) -1)
                cout << 'n';
            else
                cout << value;
        }
    }
    cout << "}";
}

void
Graph::search_print (NodeIds &nodes, queue <NodeId> nexts, unsigned int time, NodeId id, map <NodeId, NodeColor> &colors, NodeId *ancestors, NodeId *deltas, NodeId *starts, NodeId *ends)
{
    cout << " ";
    if (time == (unsigned int) -1)
        cout << "-";
    else
        cout << time;
    cout << " | ";
    if (id == (NodeId) -1)
        cout << "-";
    else
        cout << id;
    cout << " | {";
    if (deltas != 0) /* deltas is null for depth-first, not for breadth-first */
    {
        if (!nexts.empty ())
        {
            cout << nexts.front ();
            nexts.pop ();
            while (!nexts.empty ())
            {
                cout << "," << nexts.front ();
                nexts.pop ();
            }
        }
        cout << "} | {";
    }
    NodeIdIter node = nodes.begin (), node_end = nodes.end ();
    if (node != node_end)
    {
        cout << static_cast <char> (colors[*node]);
        for (++node; node != node_end; ++node)
            cout << "," << static_cast <char> (colors[*node]);
    }
    cout << "} | ";
    this->print_helper (nodes, ancestors);
    cout << " | ";
    if (deltas != 0)
    {
        this->print_helper (nodes, deltas);
        cout << " | ";
    }
    this->print_helper (nodes, starts);
    cout << " | ";
    this->print_helper (nodes, ends);
    cout << endl;
}

void
Graph::visit (NodeIds &nodes, NodeId id, unsigned int &time, map <NodeId, NodeColor> &colors, NodeId *ancestors, NodeId *starts, NodeId *ends, bool print)
{
    colors[id] = GREY;
    starts[id] = time;

    if (print)
        this->depth_first_search_print (nodes, time, id, colors, ancestors, starts, ends);

    ++time;
    NodeIds successors = this->list_successors (id);
    for (NodeIdIter successor = successors.begin (), successor_end = successors.end (); successor != successor_end; ++successor)
    {
        if (colors[*successor] == WHITE)
        {
            ancestors[*successor] = id;
            this->visit (nodes, *successor, time, colors, ancestors, starts, ends, print);
        }
    }
    colors[id] = BLACK;
    ends[id] = time;

    if (print)
        this->depth_first_search_print (nodes, time, id, colors, ancestors, starts, ends);

    ++time;
}

void
Graph::depth_first_search (NodeId start, bool print)
{
    NodeIds nodes = this->list_nodes ();
    unsigned int count = nodes.size ();
    map <NodeId, NodeColor> colors;
    NodeId *ancestors = new NodeId[count];
    unsigned int *starts = new unsigned int[count];
    unsigned int *ends = new unsigned int[count];
    for (NodeIdIter node = nodes.begin (), node_end = nodes.end (); node != node_end; ++node)
    {
        ancestors[*node] = -1;
        starts[*node] = -1;
        ends[*node] = -1;
        colors[*node] = WHITE;
    }
    unsigned int time = 0;

    bool explore_all = (start == (NodeId) -1);
    NodeIdIter start_iter = nodes.begin ();
    if (explore_all)
        start = *start_iter;

    for (NodeIdIter node = start_iter, node_end = nodes.end (); node != node_end;)
    {
        ancestors[start] = start;

        if (print)
            this->depth_first_search_print (nodes, -1, -1, colors, ancestors, starts, ends);

        this->visit (nodes, start, time, colors, ancestors, starts, ends, print);

        if (!explore_all)
            break;
        while (++node != node_end && colors[*node] == BLACK);
        if (node == node_end)
            break;
        start = *node;
    }

    delete[] (ancestors);
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
    NodeId *ancestors = new NodeId[count];
    unsigned int *deltas = new unsigned int[count];
    unsigned int *starts = new unsigned int[count];
    unsigned int *ends = new unsigned int[count];
    for (NodeIdIter node = nodes.begin (), node_end = nodes.end (); node != node_end; ++node)
    {
        ancestors[*node] = -1;
        deltas[*node] = -1;
        starts[*node] = -1;
        ends[*node] = -1;
        colors[*node] = WHITE;
    }
    unsigned int time = 0;

    bool explore_all = (start == (NodeId) -1);
    NodeIdIter start_iter = nodes.begin ();
    if (explore_all)
        start = *start_iter;

    for (NodeIdIter node = start_iter, node_end = nodes.end (); node != node_end;)
    {
        ancestors[start] = start;
        colors[start] = GREY;
        deltas[start] = 0;
        nexts.push (start);

        if (print)
            this->search_print (nodes, nexts, -1, -1, colors, ancestors, deltas, starts, ends);

        while (!nexts.empty ())
        {
            NodeId id = nexts.front ();
            nexts.pop ();
            starts[id] = time;
            NodeIds successors = this->list_successors (id);
            for (NodeIdIter successor = successors.begin (), successor_end = successors.end (); successor != successor_end; ++successor)
            {
                if (colors[*successor] == WHITE)
                {
                    colors[*successor] = GREY;
                    ancestors[*successor] = id;
                    deltas[*successor] = deltas[id] + 1;
                    nexts.push (*successor);
                }
            }

            if (print)
                this->search_print (nodes, nexts, time, id, colors, ancestors, deltas, starts, ends);

            ++time;
            colors[id] = BLACK;
            ends[id] = time;

            if (print)
                this->search_print (nodes, nexts, time, id, colors, ancestors, deltas, starts, ends);

            ++time;
        }
        if (!explore_all)
            break;
        while (++node != node_end && colors[*node] == BLACK);
        if (node == node_end)
            break;
        start = *node;
    }

    delete[] (ancestors);
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

bool
Graph::path_exists_between (NodeId from, NodeId to)
{
    // TODO: use to to exit earlier
    // TODO: nodes.size () is not what we want, same for searches
    queue <NodeId> nexts;
    NodeIds nodes = this->list_nodes ();
    NodeId *ancestors = new NodeId[nodes.size ()];
    unsigned int *deltas = new unsigned int[nodes.size ()];
    for (NodeIdIter node = nodes.begin (), node_end = nodes.end (); node != node_end; ++node)
    {
        ancestors[*node] = -1;
        deltas[*node] = -1;
        nexts.push (*node);
    }
    // We do not want to set the ancestor of from to itself yet since we want to detect cycles
    deltas[from] = 0;
    while (!nexts.empty ())
    {
        NodeId node = nexts.front ();
        nexts.pop ();
        ArcIds arcs = this->list_arcs_from (node);
        for (ArcIdIter arc = arcs.begin (), arc_end = arcs.end (); arc != arc_end; ++arc)
        {
            ArcId arc_destination = this->arcs[*arc].second;
            if (deltas[arc_destination] > deltas[node])
            {
                ancestors[arc_destination] = node;
                deltas[arc_destination] = deltas[node] + 1;
            }
        }
    }
    while ((to = ancestors[to]) != (NodeId) -1 && to != from);
    delete[] (ancestors);
    delete[] (deltas);
    return (to == from);
}

bool
Graph::acyclic ()
{
    // TODO
    return true;
}

bool
Graph::simple ()
{
    if (!this->acyclic ())
        return false;

    NodeIds nodes = this->list_nodes ();
    for (NodeIdIter from = nodes.begin (), from_end = nodes.end (); from != from_end; ++from)
    {
        for (NodeIdIter to = nodes.begin (), to_end = nodes.end (); to != to_end; ++to)
        {
            if (this->list_arcs_from_to (*from, *to).size () > 1)
                return false;
        }
    }

    return true;
}
