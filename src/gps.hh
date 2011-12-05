#ifndef __GPS_H__
#define __GPS_H__

#include "digraph/digraph.hh"

#include <map>

using std::map;
using std::vector;

class Gps
{
public:
    Gps (DiGraph *_graph, string path, double _coeff, string start, string dest);
    virtual ~Gps() {}

    void calculate_by_agregation ();
    void calculate_by_bounded_detour ();

    class MapElem {
    public:
        string label; // or id ?
        int interest;
    };
    class City : public MapElem {};
    class Road : public MapElem {
    public:
        double length;
    };
private:
    typedef union {
        NodeId city;
        ArcId road;
    } PathElem;

    NodeId start_node;
    NodeId end_node;
    NodeIds nodes_list;
    unsigned int nodes_count;

    list <PathElem> best_path;
    double best_distance;
    int best_interest;

    double shortest_path (NodeId from, NodeId to);
    void visit (NodeId id, map <NodeId, NodeColor> &colors, NodeId *ancestors, double current_distance, int current_interest, list <Gps::PathElem> &current_path);

    DiGraph *graph;
    map <string, NodeId> nodes;
    map <NodeId, City> cities;
    map <ArcId, Road> roads;

    double coeff;
    double max_allowed;
    double dmax;
    int imax;
};

#endif /*__GPS_H__*/
