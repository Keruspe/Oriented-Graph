#include "gps.hh"
#include "map-parser.hh"

#include <queue>

using std::queue;

Gps::Gps (DiGraph *_graph, string path, double _K, double _A) :
    graph (_graph),
    K (_K),
    A (_A)
{
    MapParser parser(_graph, path, this->nodes, this->cities, this->roads, &this->dmax, &this->imax);
}

void
Gps::calculate_by_agregation (string start, string dest)
{
    NodeId start_node = this->nodes[start];
    NodeId end_node = this->nodes[dest];
}

void
Gps::visit (NodeIds &nodes, NodeId id, map <NodeId, NodeColor> &colors, NodeId *ancestors, double distance, double max_allowed, NodeId dest, int current_interest, int *best_interest, list <Gps::PathElem> current, list<Gps::PathElem> &best)
{
    colors[id] = GREY;

    ArcIds successors = this->graph->list_arcs_from (id);
    for (ArcIdIter successor = successors.begin (), successor_end = successors.end (); successor != successor_end; ++successor)
    {
        NodeId succ = this->graph->get_arc_details (*successor).second;
        double road_length = this->roads[*successor].length;
        if (colors[succ] == WHITE && distance + road_length <= max_allowed)
        {
            ancestors[succ] = id;
            PathElem tmp;
            tmp.road = *successor;
            current.push_back (tmp);
            tmp.city = succ;
            current.push_back (tmp);
            if (succ != dest)
                this->visit (nodes, succ, colors, ancestors, distance + road_length, max_allowed, dest, current_interest + this->roads[*successor].interest + this->cities[succ].interest, best_interest, current, best);
            else
            {
                int cur_interest = current_interest + this->roads[*successor].interest + this->cities[succ].interest;
                if (cur_interest > *best_interest)
                {
                    *best_interest = cur_interest;
                    best = current;
                }
            }
        }
    }

    colors[id] = WHITE;
}

void
Gps::calculate_by_bounded_detour (string start, string dest)
{
    NodeId start_node = this->nodes[start];
    NodeId end_node = this->nodes[dest];
    NodeIds nodes_list = this->graph->list_nodes ();
    unsigned int count = nodes_list.size ();
    double shortest_path = this->shortest_path (nodes_list, count, start_node, end_node);
    double max_allowed = this->K * shortest_path;
    cout << "Shortest distance: " << shortest_path << "km, maximum allowed: " << max_allowed << "km" << endl;

    map <NodeId, NodeColor> colors;
    NodeId *ancestors = new NodeId[count];
    for (NodeIdIter node = nodes_list.begin (), node_end = nodes_list.end (); node != node_end; ++node)
    {
        ancestors[*node] = -1;
        colors[*node] = WHITE;
    }
    ancestors[start_node] = start_node;
    int start_interest = this->cities[start_node].interest;
    int best_interest = start_interest;
    PathElem tmp;
    tmp.city = start_node;
    list <PathElem> current;
    current.push_back (tmp);
    list <PathElem> best;
    this->visit (nodes_list, start_node, colors, ancestors, 0, max_allowed, end_node, start_interest, &best_interest, current, best);

    cout << "Best interest: " << best_interest << endl;

    delete[] (ancestors);
}

double
Gps::shortest_path (NodeIds &nodes_list, unsigned int count, NodeId from, NodeId to)
{
    NodeId *ancestors = new NodeId[count];
    double *deltas = new double[count];
    queue <NodeId> nexts; // We don't use a priority queue here since we do not ponderate nodes for now.
    map <NodeId, NodeColor> colors; // To track which nodes to explore
    for (NodeIdIter node = nodes_list.begin (), node_end = nodes_list.end (); node != node_end; ++node)
    {
        deltas[*node] = -1;
        ancestors[*node] = -1;
        colors[*node] = WHITE;
    }
    deltas[from] = 0;
    ancestors[from] = from;
    nexts.push (from);
    colors[from] = BLACK;
    while (!nexts.empty ())
    {
        NodeId node = nexts.front ();
        nexts.pop ();
        ArcIds arcs = this->graph->list_arcs_from (node);
        for (ArcIdIter arc = arcs.begin (), arc_end = arcs.end (); arc != arc_end; ++arc)
        {
            ArcId arc_destination = this->graph->get_arc_details(*arc).second;
            if (colors[arc_destination] == WHITE)
            {
                nexts.push (arc_destination);
                colors[arc_destination] = BLACK;
            }
            double distance = this->roads[*arc].length;
            if (deltas[arc_destination] == -1 || deltas[arc_destination] > deltas[node] + distance) 
            {
                ancestors[arc_destination] = node;
                deltas[arc_destination] = deltas[node] + distance;
            }
        }
    }
    double ret = deltas[to];
    delete[] (ancestors);
    delete[] (deltas);
    return ret;
}
