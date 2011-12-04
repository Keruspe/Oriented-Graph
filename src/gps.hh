#ifndef __GPS_H__
#define __GPS_H__

#include "digraph/digraph.hh"

#include <map>

using std::map;
using std::vector;

class Gps
{
public:
    Gps (DiGraph *_graph, string path, double _K, double _A);
    virtual ~Gps() {}

    void calculate_by_agregation (string start, string dest);
    void calculate_by_bounded_detour (string start, string dest);

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

    double shortest_path (NodeIds &nodes_list, unsigned int count, NodeId from, NodeId to);
    void visit (NodeIds &nodes, NodeId id, map <NodeId, NodeColor> &colors, NodeId *ancestors, double distance, double max_allowed, NodeId dest, int current_interest, int *best_interest, list <PathElem> current, list<PathElem> &best, int *best_distance);

    DiGraph *graph;
    map <string, NodeId> nodes;
    map <NodeId, City> cities;
    map <ArcId, Road> roads;
    double K;
    double A;
    double dmax;
    int imax;
};

#endif /*__GPS_H__*/
