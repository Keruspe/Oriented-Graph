#ifndef __MAP_PARSER_H__
#define __MAP_PARSER_H__

#include "digraph/digraph.hh"
#include "gps.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <map>

using std::map;
using std::ifstream;

class MapParser
{
public:
    MapParser (DiGraph *_graph, string pathi, map <string, NodeId> &nodes, map <NodeId, Gps::City> cities, map <ArcId, Gps::Road> &roads, NodeIds &nodes_list, ArcIds &arcs_list, double *dmax, int *imax);
    virtual ~MapParser () {}
private:
    DiGraph *graph;
    ifstream *map_file;
    string token;

    string id_or_from;
    string dest;
    string label;
    int interest;
    string name;
    double length;

    string next_token ();
    bool expect (string expected);
    void handle_label ();
};

#endif /*__MAP_PARSER_H__*/
