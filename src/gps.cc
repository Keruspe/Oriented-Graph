#include "gps.hh"
#include "map-parser.hh"

#include <queue>
#include <vector>

using std::queue;

	// Type des données stockées dans la file :
	class Elem
	{
		public:
		int k;         // Priorité
		NodeId t; // Valeur
		Elem() : k(0), t(0) {}
		Elem(int k, const NodeId t) : k(k), t(t) {}
	};

	// Foncteur de comparaison selon les priorités :
	class Compare
	{
		public:
			bool operator()(const Elem &a1, const Elem &a2)
			{
				return a1.k < a2.k ;
			}
	};

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
	//TODO: nettoyer

    NodeId start_node = this->nodes[start];
    NodeId end_node = this->nodes[dest];
    
    //@debug
    //FIXME : e=start
    std::cout<< "start:"<<this->nodes[start]<<" dest:"<<this->nodes[dest] <<std::endl;
	
	int i, j;
	//TODO : optimize, clean
	std::vector<int> d(1000);
	std::vector<int> father(1000);
	std::vector<bool> visit(1000);
	
	std::map<string, NodeId>::iterator it = nodes.begin();
	
	std::priority_queue<Elem, vector<Elem>, Compare> pq;
	for (; it != nodes.end();it++)
	{
		Elem e(0, (it->second));
		pq.push(e);
		d[it->second]=999;
	}
	
	while(!pq.empty())
	{
		NodeId u = pq.top().t;
		pq.pop();
		
		std::list<NodeId> successors = graph->list_successors(u);
		for(std::list<NodeId>::iterator it = successors.begin();it!=successors.end();it++)
		{
			if(visit.at(*it)==false)
			{
				visit.at(*it)=true;
				if(d[*it]>d[u]+1)//FIXME: 1=>fonction de poids a faire
				{
					d[*it] = d[u]+1;//FIXME: 1=>fonction de poids a faire
					father[*it]=u;
				}
			}
		}
	}

}

void
Gps::visit (NodeIds &nodes, NodeId id, map <NodeId, NodeColor> &colors, NodeId *ancestors, double distance, double max_allowed, NodeId dest, int current_interest, int *best_interest, list <Gps::PathElem> current, list<Gps::PathElem> &best, int *best_distance)
{
    colors[id] = GREY;

    ArcIds successors = this->graph->list_arcs_from (id);
    for (ArcIdIter successor = successors.begin (), successor_end = successors.end (); successor != successor_end; ++successor)
    {
        NodeId succ = this->graph->get_arc_details (*successor).second;
        double road_length = this->roads[*successor].length;
        if (colors[succ] == WHITE && distance + road_length <= max_allowed)
        {
            list <Gps::PathElem> current_copy = current;
            ancestors[succ] = id;
            PathElem tmp;
            tmp.road = *successor;
            current_copy.push_back (tmp);
            tmp.city = succ;
            current_copy.push_back (tmp);
            if (succ != dest)
                this->visit (nodes, succ, colors, ancestors, distance + road_length, max_allowed, dest, current_interest + this->roads[*successor].interest + this->cities[succ].interest, best_interest, current_copy, best, best_distance);
            else
            {
                int cur_interest = current_interest + this->roads[*successor].interest + this->cities[succ].interest;
                if (cur_interest > *best_interest)
                {
                    *best_interest = cur_interest;
                    best = current_copy;
                    *best_distance = distance + road_length;
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
    cout << endl
        << "Shortest distance: " << shortest_path << "km" << endl
        << "Maximum allowed: " << max_allowed << "km" << endl << endl;

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
    int best_distance = 0;
    PathElem tmp;
    tmp.city = start_node;
    list <PathElem> current;
    current.push_back (tmp);
    list <PathElem> best;
    this->visit (nodes_list, start_node, colors, ancestors, 0, max_allowed, end_node, start_interest, &best_interest, current, best, &best_distance);

    bool is_city = true;
    for (list <PathElem>::iterator elem = best.begin (), elem_end = best.end (); elem != elem_end; ++elem, is_city = !is_city)
    {
        if (is_city)
            cout << "City: " << this->cities[elem->city].label << endl;
        else
            cout << "Road: " << this->roads[elem->road].label << endl;
    }
    cout << endl
        << "Total distance: " << best_distance << endl
        << "Total interest: " << best_interest << endl;

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
