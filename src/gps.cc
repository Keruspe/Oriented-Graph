#include "gps.hh"
#include "map-parser.hh"

Gps::Gps (DiGraph *_graph, string path, int _K, int _A) :
    graph (_graph),
    K (_K),
    A (_A)
{
    MapParser parser(_graph, path, nodes, cities, roads);
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
}
