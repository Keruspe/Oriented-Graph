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

Gps::Gps (DiGraph *_graph, string path, double _coeff, string start, string dest) :
    graph (_graph),
    coeff (_coeff)
{
    MapParser parser(_graph, path, this->nodes, this->cities, this->roads, &this->dmax, &this->imax);
    this->start_node = this->nodes[start];
    this->end_node = this->nodes[dest];
    this->nodes_list = this->graph->list_nodes ();
    this->nodes_count = this->nodes_list.size ();
}

void
Gps::calculate_by_agregation ()
{
	map<ArcId, Road>::iterator itRoads;
	
	// Calcul des maximums pour la fonction de pondération
	double maxDistance=0.;
	int maxInterest=0;
	for(itRoads = roads.begin(); itRoads != roads.end(); itRoads++){
		if(maxDistance<itRoads->second.length)
			maxDistance=itRoads->second.length;
		if(maxInterest<itRoads->second.interest)
			maxInterest=itRoads->second.interest;
	}
	
	map<NodeId, City>::iterator itCities;
	for(itCities = cities.begin(); itCities != cities.end(); itCities++){
		if(maxInterest<itCities->second.interest)
			maxInterest=itCities->second.interest;
	}
		
	//initialisation
	std::vector<double> weight;
	std::vector<NodeId> preds;
	std::vector<double> distance;
	std::vector<ArcId> roadsTaken;
	std::vector<int> interestsSeen;
	
	//!initialisation
	for (int i = 0; i < roads.size(); i++) {
		weight.insert(weight.begin()+i,9999.0);//*max
    }
	for (int i = 0; i < cities.size(); i++) {
		preds.insert(preds.begin()+i, -1); //*changer -1 pour null ou un truc du genre
		distance.insert(distance.begin()+i, 0);
		roadsTaken.insert(roadsTaken.begin()+i, -1);
		interestsSeen.insert(interestsSeen.begin()+i, 0);
    }
    weight.at(this->start_node)=0.0;
    preds.at(this->start_node) = this->start_node;
    //!fin_init
    
    for (NodeIdIter node = nodes_list.begin(), node_end = nodes_list.end (); node != node_end; ++node)
    {
		NodeId i = *node; 
		if (i != -1) {
			ArcIds arcs_sortants = graph->list_arcs_from(i);
			
			map<ArcId, Road>::iterator it;

			for(it = roads.begin(); it != roads.end(); ++it){
				NodeId source = graph->get_arc_details((*it).first).first;
				NodeId dest = graph->get_arc_details((*it).first).second;

				if(weight[source]+it->second.length<weight[dest])
				{
					//~ std::cout<< "arclength: "<<it->second.length <<"distance[dest]="<<distance[dest]<<std::endl;
					double dummy = weight[source] +  coeff*it->second.length/maxDistance-(1-coeff)*(it->second.interest+cities[dest].interest)/(2*maxInterest);
					if (dummy < weight[dest])
					{
						weight[dest] = dummy;
						preds[dest] = source;
						distance[dest] = it->second.length;
						roadsTaken[dest] = (*it).first;
						interestsSeen[dest] = it->second.interest+cities[dest].interest;
					}
				}
			}
		}
	}
	
	//~ for (int i=0; i < edgenum; ++i) {
         //~ if (distance[edges[i].destination] > distance[edges[i].source] + edges[i].weight) {
             //~ std::cout<<"Negative edge weight cycles detected!\n";
             //~ free(distance);
             //~ break;
         //~ }//end if
     //~ }//end for
	
	bool done=false;
	this->best_distance=0;
	this->best_interest=0;
	NodeId current_node = this->end_node;
	
	while(done == false)
	{
		std::cout<< "City:"<<	cities[current_node].label				<<	std::endl;
		std::cout<< "Road:"<<	roads[roadsTaken[current_node]].label	<<	std::endl;
		
		this->best_distance += distance[current_node];
		this->best_interest += interestsSeen[current_node];
		current_node = preds[current_node];
		
		if(current_node==this->start_node)
			done=true;
	}
	std::cout<< 	"City:"<<	cities[current_node].label <<	std::endl;
		
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
Gps::calculate_by_bounded_detour ()
{
    double shortest_path = this->shortest_path (this->start_node, this->end_node);
    this->max_allowed = this->coeff * shortest_path;

    cout << "Shortest distance: " << shortest_path << "km" << endl
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
    this->best_distance = 0;
    PathElem tmp;
    tmp.city = this->start_node;
    list <PathElem> current;
    current.push_back (tmp);
    this->visit (this->start_node, colors, ancestors, this->best_distance, start_interest, current);

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

void
Gps::print_result ()
{
    bool is_city = true;
    for (list <PathElem>::iterator elem = this->best_path.begin (), elem_end = this->best_path.end (); elem != elem_end; ++elem, is_city = !is_city)
    {
        if (is_city)
            cout << "City: " << this->cities[elem->city].label << endl;
        else
            cout << "Road: " << this->roads[elem->road].label << endl;
    }
    cout << endl
        << "Total distance: " << this->best_distance << "km" << endl
        << "Total interest: " << this->best_interest << endl;
}
