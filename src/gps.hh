#ifndef __GPS_H__
#define __GPS_H__

#include "digraph/digraph.hh"

#include <map>

using std::map;
using std::vector;

class Gps
{
public:
    Gps (DiGraph *_graph, string path, int _K, int _A);
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
    DiGraph *graph;
    map <string, NodeId> nodes;
    map <NodeId, City> cities;
    map <ArcId, Road> roads;
    NodeIds nodes_list;
    ArcIds arcs_list;
    double K;
    double A;
    double dmax;
    int imax;
};

#endif /*__GPS_H__*/
