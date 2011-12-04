#include "gps.hh"
#include "map-parser.hh"

#include <queue>

using std::queue;

Gps::Gps (DiGraph *_graph, string path, double _K, double _A) :
    graph (_graph),
    K (_K),
    A (_A)
{
    MapParser parser(_graph, path, this->nodes, this->cities, this->roads, this->nodes_list, this->arcs_list, &this->dmax, &this->imax);
}

void
Gps::calculate_by_agregation (string start, string dest)
{
    NodeId start_node = this->nodes[start];
    NodeId end_node = this->nodes[dest];
}

void
Gps::calculate_by_bounded_detour (string start, string dest)
{
    NodeId start_node = this->nodes[start];
    NodeId end_node = this->nodes[dest];
    double shortest_path = this->shortest_path (start_node, end_node);
    double max_allowed = this->K * shortest_path;
    cout << "Shortest distance: " << shortest_path << "km, maximum allowed: " << max_allowed << "km" << endl;
}

double
Gps::shortest_path (NodeId from, NodeId to)
{
    unsigned int count = this->nodes_list.size ();
    NodeId *ancestors = new NodeId[count];
    double *deltas = new double[count];
    queue <NodeId> nexts; // We don't use a priority queue here since we do not ponderate nodes for now.
    map <NodeId, NodeColor> colors; // To track which nodes to explore
    for (NodeIdIter node = this->nodes_list.begin (), node_end = this->nodes_list.end (); node != node_end; ++node)
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
