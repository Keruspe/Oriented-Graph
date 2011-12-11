#include "digraph.hh"

#include <sstream>

ArcId
DiGraph::get_new_arc_id (NodeId from, NodeId to)
{
    /* Store arcs details (from, to) */
    this->arcs[this->_next_arc_id] = std::make_pair <NodeId, NodeId> (from, to);
    return this->_next_arc_id++;
}

void
DiGraph::print_helper (NodeIds &nodes, unsigned int *data)
{
    /* This is just an helper not to duplicate printing code */
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
DiGraph::search_print (NodeIds &nodes, queue <NodeId> nexts, unsigned int time, NodeId id, map <NodeId, NodeColor> &colors, NodeId *ancestors, NodeId *deltas, NodeId *starts, NodeId *ends)
{
    /* This function prints each search step as a table line */
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
DiGraph::visit (NodeIds &nodes, NodeId id, unsigned int &time, map <NodeId, NodeColor> &colors, NodeId *ancestors, NodeId *starts, NodeId *ends, bool print, NodeIds &nodes_explored)
{
    /* The recursive method for depth-first search */
    nodes_explored.push_back (id);
    colors[id] = GREY;
    starts[id] = time;

    if (print)
        this->depth_first_search_print (nodes, time, id, colors, ancestors, starts, ends);

    ++time;
    NodeIds successors = this->list_successors (id);
    /* Go through all successors of the current node and visit them */
    for (NodeIdIter successor = successors.begin (), successor_end = successors.end (); successor != successor_end; ++successor)
    {
        if (colors[*successor] == WHITE)
        {
            ancestors[*successor] = id;
            this->visit (nodes, *successor, time, colors, ancestors, starts, ends, print, nodes_explored);
        }
    }
    colors[id] = BLACK;
    ends[id] = time;

    if (print)
        this->depth_first_search_print (nodes, time, id, colors, ancestors, starts, ends);

    ++time;
}

void
DiGraph::depth_first_search (NodeId start, bool print)
{
    /* Basic initialization of the algorithm */
    NodeIds nodes = this->list_nodes ();
    unsigned int count = this->next_node_id ();
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

    /* if we're given -1 as the start node, then we'll do an exhaustive depth-first search */
    bool explore_all = (start == (NodeId) -1);
    NodeIdIter start_iter = nodes.begin ();
    if (explore_all)
        start = *start_iter;

    NodeIds nodes_explored;
    /* loop across all the nodes, for explore_all */
    for (NodeIdIter node = start_iter, node_end = nodes.end (); node != node_end;)
    {
        ancestors[start] = start;

        if (print)
            this->depth_first_search_print (nodes, -1, -1, colors, ancestors, starts, ends);

        this->visit (nodes, start, time, colors, ancestors, starts, ends, print, nodes_explored);

        /* If we're not doing an exhaustive search, we're done */
        if (!explore_all)
            break;
        /* find next WHITE node */
        while (++node != node_end && colors[*node] == BLACK);
        if (node == node_end)
            break;
        start = *node;
    }

    cout << endl << "Nodes explored:";
    for (NodeIdIter node = nodes_explored.begin (), node_end = nodes_explored.end (); node != node_end; ++node)
        cout << " " << *node;
    cout << endl;

    delete[] (ancestors);
    delete[] (starts);
    delete[] (ends);
}

void
DiGraph::breadth_first_search (NodeId start, bool print)
{
    /* Basic initialization of the algorithm */
    NodeIds nodes = this->list_nodes ();
    unsigned int count = this->next_node_id ();
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

    /* if we're given -1 as the start node, then we'll do an exhaustive depth-first search */
    bool explore_all = (start == (NodeId) -1);
    NodeIdIter start_iter = nodes.begin ();
    if (explore_all)
        start = *start_iter;

    NodeIds nodes_explored;
    /* loop across all the nodes, for explore_all */
    for (NodeIdIter node = start_iter, node_end = nodes.end (); node != node_end;)
    {
        ancestors[start] = start;
        colors[start] = GREY;
        deltas[start] = 0;
        nexts.push (start);

        if (print)
            this->search_print (nodes, nexts, -1, -1, colors, ancestors, deltas, starts, ends);

        /* Loop while we still have nodes to deal with */
        while (!nexts.empty ())
        {
            NodeId id = nexts.front ();
            nexts.pop ();
            nodes_explored.push_back (id);
            starts[id] = time;
            NodeIds successors = this->list_successors (id);
            /* Mark all WHITE successors as nodes we have to deal with */
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
        /* If we're not doing an exhaustive search, we're done */
        if (!explore_all)
            break;
        /* find next WHITE node */
        while (++node != node_end && colors[*node] == BLACK);
        if (node == node_end)
            break;
        start = *node;
    }

    cout << endl << "Nodes explored:";
    for (NodeIdIter node = nodes_explored.begin (), node_end = nodes_explored.end (); node != node_end; ++node)
        cout << " " << *node;
    cout << endl;

    delete[] (ancestors);
    delete[] (deltas);
    delete[] (starts);
    delete[] (ends);
}

bool
DiGraph::connex ()
{
    NodeIds nodes = this->list_nodes ();
    /* Go through all nodes */
    for (NodeIdIter node = nodes.begin (), node_end = nodes.end (); node != node_end; ++node)
    {
        ArcIds arcs = this->list_arcs_to (*node);
        bool reachable = false;
        /* Check if the arcs (if any) are not just loops a->a */
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
            /* If arc is not reachable, it has to reach other nodes */
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

pair <NodeId, NodeId>
DiGraph::get_arc_details (ArcId id)
{
    /* returns (from, to) */
    return this->arcs[id];
}

bool
DiGraph::path_exists_between (NodeIds &nodes, NodeId from, NodeId to)
{
    /* Helper to check if there is a path between 2 nodes. */
    unsigned int count = this->next_node_id ();
    NodeId *ancestors = new NodeId[count];
    unsigned int *deltas = new unsigned int[count];
    for (NodeIdIter node = nodes.begin (), node_end = nodes.end (); node != node_end; ++node)
    {
        ancestors[*node] = -1;
        deltas[*node] = -1;
    }
    // We do not want to set the ancestor of from to itself yet since we want to detect cycles
    deltas[from] = 0;
    /* Go through the nodes to store their more direct ancestors */
    for (NodeIdIter node = nodes.begin (), node_end = nodes.end (); node != node_end; ++node)
    {
        ArcIds arcs = this->list_arcs_from (*node);
        for (ArcIdIter arc = arcs.begin (), arc_end = arcs.end (); arc != arc_end; ++arc)
        {
            ArcId arc_destination = this->arcs[*arc].second;
            if (deltas[arc_destination] > deltas[*node] || (arc_destination == from && deltas[from] == 0))
            {
                ancestors[arc_destination] = *node;
                deltas[arc_destination] = deltas[*node] + 1;
            }
        }
    }
    /* go back through the ancestors history to try to find the "from" node */
    while ((to = ancestors[to]) != (NodeId) -1 && to != from);
    delete[] (ancestors);
    delete[] (deltas);
    return (to == from);
}

bool
DiGraph::acyclic ()
{
    /* We check if a node can reach itself, or none */
    NodeIds nodes = this->list_nodes ();
    for (NodeIdIter node = nodes.begin (), node_end = nodes.end (); node != node_end; ++node)
    {
        if (this->path_exists_to_self (nodes, *node))
            return false;
    }
    return true;
}

bool
DiGraph::simple ()
{
    if (!this->acyclic ())
        return false;

    /* Check if there are 2 nodes with more than 1 arc between them */
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

string
DiGraph::as_matrix ()
{
    /* display as adjacency matrix */
    std::ostringstream out;
    out << " ";
    NodeIds nodes = this->list_nodes ();
    for (NodeIdIter node = nodes.begin (), node_end = nodes.end (); node != node_end; ++node)
        out << "   " << *node;
    out << endl;
    for (NodeIdIter from = nodes.begin (), from_end = nodes.end (); from != from_end; ++from)
    {
        out << *from;
        for (NodeIdIter to = nodes.begin (), to_end = nodes.end (); to != to_end; ++to)
            out << "   " << this->list_arcs_from_to (*from, *to).size ();
        out << endl;
    }
    return out.str ();
}

string
DiGraph::as_list ()
{
    /* display as adjacency list */
    std::ostringstream out;
    NodeIds nodes = this->list_nodes ();
    for (NodeIdIter from = nodes.begin (), from_end = nodes.end (); from != from_end; ++from)
    {
        out << *from << " -> ";
        ArcIds arcs = this->list_arcs_from (*from);
        ArcIdIter arc = arcs.begin (), arc_end = arcs.end ();
        if (arc == arc_end)
            out << "-";
        else
        {
            out << this->arcs[*arc].second;
            for (++arc; arc != arc_end; ++arc)
                out << "," << this->arcs[*arc].second;
        }
        out << endl;
    }
    return out.str ();
}
