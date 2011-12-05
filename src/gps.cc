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

Gps::Gps (DiGraph *_graph, string path, double _coeff) :
    graph (_graph),
    coeff (_coeff)
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
Gps::visit (NodeId id, map <NodeId, NodeColor> &colors, NodeId *ancestors, double current_distance, int current_interest, list <Gps::PathElem> &current_path)
{
    colors[id] = GREY;

    ArcIds arcs = this->graph->list_arcs_from (id);
    for (ArcIdIter arc = arcs.begin (), arc_end = arcs.end (); arc != arc_end; ++arc)
    {
        NodeId successor = this->graph->get_arc_details (*arc).second;
        Road road = this->roads[*arc];
        double road_length = road.length;
        if (colors[successor] == WHITE && current_distance + road_length <= this->max_allowed)
        {
            list <Gps::PathElem> current_copy = current_path;
            ancestors[successor] = id;
            PathElem tmp;
            tmp.road = *arc;
            current_copy.push_back (tmp);
            tmp.city = successor;
            current_copy.push_back (tmp);
            if (successor != this->end_node)
                this->visit (successor, colors, ancestors, current_distance + road_length, current_interest + road.interest + this->cities[successor].interest, current_copy);
            else
            {
                int cur_interest = current_interest + road.interest + this->cities[successor].interest;
                if (cur_interest > this->best_interest)
                {
                    this->best_interest = cur_interest;
                    this->best_path = current_copy;
                    this->best_distance = current_distance + road_length;
                }
            }
        }
    }

    colors[id] = WHITE;
}

void
Gps::calculate_by_bounded_detour (string start, string dest)
{
    this->start_node = this->nodes[start];
    this->end_node = this->nodes[dest];
    this->nodes_list = this->graph->list_nodes ();
    this->nodes_count = this->nodes_list.size ();
    double shortest_path = this->shortest_path (this->start_node, this->end_node);
    this->max_allowed = this->coeff * shortest_path;

    cout << endl
        << "Shortest distance: " << shortest_path << "km" << endl
        << "Maximum allowed: " << this->max_allowed << "km" << endl << endl;

    map <NodeId, NodeColor> colors;
    NodeId *ancestors = new NodeId[this->nodes_count];
    for (NodeIdIter node = this->nodes_list.begin (), node_end = this->nodes_list.end (); node != node_end; ++node)
    {
        ancestors[*node] = -1;
        colors[*node] = WHITE;
    }
    ancestors[this->start_node] = this->start_node;
    int start_interest = this->best_interest = this->cities[start_node].interest;
    PathElem tmp;
    tmp.city = this->start_node;
    list <PathElem> current;
    current.push_back (tmp);
    this->visit (this->start_node, colors, ancestors, 0, start_interest, current);

    bool is_city = true;
    for (list <PathElem>::iterator elem = this->best_path.begin (), elem_end = this->best_path.end (); elem != elem_end; ++elem, is_city = !is_city)
    {
        if (is_city)
            cout << "City: " << this->cities[elem->city].label << endl;
        else
            cout << "Road: " << this->roads[elem->road].label << endl;
    }
    cout << endl
        << "Total distance: " << this->best_distance << endl
        << "Total interest: " << this->best_interest << endl;

    delete[] (ancestors);
}

double
Gps::shortest_path (NodeId from, NodeId to)
{
    NodeId *ancestors = new NodeId[this->nodes_count];
    double *deltas = new double[this->nodes_count];
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
