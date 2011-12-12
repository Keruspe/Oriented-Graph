#include "gps.hh"
#include "map-parser.hh"

#include <queue>
#include <vector>

using std::queue;

Gps::Gps (DiGraph *_graph, string path, double _coeff, string start, string dest) :
    graph (_graph),
    coeff (_coeff)
{
    /* Parse the map to get all the data */
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
	double maxDistance=this->dmax;
	int maxInterest=this->imax;

	//initialisation
	std::vector<double> weight;
	std::vector<NodeId> preds;
	std::vector<double> distance;
	std::vector<ArcId> roadsTaken;
	std::vector<int> interestsSeen;

	//initialization begins
	for (unsigned int i = 0; i < roads.size(); i++) {
		weight.insert(weight.begin()+i,9999999.0);
    }
	for (unsigned int i = 0; i < cities.size(); i++) {
		preds.insert(preds.begin()+i, -1); 
		distance.insert(distance.begin()+i, 0);
		roadsTaken.insert(roadsTaken.begin()+i, -1);
		interestsSeen.insert(interestsSeen.begin()+i, 0);
    }
    weight.at(this->start_node)=0.0;
    preds.at(this->start_node) = this->start_node;
    //initialization ends
    
	/* nodes iteration */
    for (NodeIdIter node = nodes_list.begin(), node_end = nodes_list.end (); node != node_end; ++node)
    {
		NodeId i = *node; 
		if (i != (NodeId) -1) {
			ArcIds arcs_sortants = graph->list_arcs_from(i);

			map<ArcId, Road>::iterator it;

			for(it = roads.begin(); it != roads.end(); ++it){
				NodeId source = graph->get_arc_details((*it).first).first;
				NodeId dest = graph->get_arc_details((*it).first).second;
				
				/* weight function */
				double tempweight = coeff*it->second.length/maxDistance-(1-coeff)*(it->second.interest+cities[dest].interest)/(2*maxInterest);
				
				/* weight comparison */
				if(weight[source]+tempweight<weight[dest])
				{
					
					double dummy = weight[source] +  tempweight;
					if (dummy < weight[dest])
					{
						/* tables updates */
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
	
	/* printing initialization */
	bool done=false;
	this->best_distance=0;
	this->best_interest=0;
	NodeId current_node = this->end_node;

	/* prepare for printing */
	std::list<MapElem> outList;
	while(done == false)
	{
		outList.push_front(cities[current_node]);
		outList.push_front(roads[roadsTaken[current_node]]);
		this->best_distance += distance[current_node];
		this->best_interest += interestsSeen[current_node];
		current_node = preds[current_node];

		if(current_node==this->start_node)
			done=true;
	}
	
	/* let's print */
	bool is_city = false;
	for(std::list<MapElem>::iterator it=outList.begin();it!=outList.end();it++,is_city = !is_city)
	{
		if(is_city)
			std::cout<< "City:"<<	(*it).label	<<	std::endl;
		else
			std::cout<< "Road:"<<	(*it).label	<<	std::endl;
	}
}

void
Gps::visit (NodeId id, map <NodeId, NodeColor> &colors, NodeId *ancestors, double current_distance, int current_interest, list <Gps::PathElem> &current_path)
{
    colors[id] = GREY;

    ArcIds arcs = this->graph->list_arcs_from (id);
    /* Explore all roads going from the current city */
    for (ArcIdIter arc = arcs.begin (), arc_end = arcs.end (); arc != arc_end; ++arc)
    {
        NodeId successor = this->graph->get_arc_details (*arc).second;
        Road road = this->roads[*arc];
        double road_length = road.length;
        /* We only give it a chance if it's not too far for the user */
        if (colors[successor] == WHITE && current_distance + road_length <= this->max_allowed)
        {
            /* Copy the current itinerary to pass it to the next recursion level, add the road and the city to it */
            list <Gps::PathElem> current_copy = current_path;
            ancestors[successor] = id;
            PathElem tmp;
            tmp.road = *arc;
            current_copy.push_back (tmp);
            tmp.city = successor;
            current_copy.push_back (tmp);
            /* Visit next city if it's not the destination */
            if (successor != this->end_node)
                this->visit (successor, colors, ancestors, current_distance + road_length, current_interest + road.interest + this->cities[successor].interest, current_copy);
            else
            {
                /* We're at destination, store as best itinerary or not. */
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
    /* Calculate maximum distance allowed */
    double shortest_path = this->shortest_path (this->start_node, this->end_node);
    this->max_allowed = this->coeff * shortest_path;

    cout << "Shortest distance: " << shortest_path << "km" << endl
        << "Maximum allowed: " << this->max_allowed << "km" << endl << endl;

    /* initilization of the algorithm */
    map <NodeId, NodeColor> colors;
    NodeId *ancestors = new NodeId[this->nodes_count];
    for (NodeIdIter node = this->nodes_list.begin (), node_end = this->nodes_list.end (); node != node_end; ++node)
    {
        ancestors[*node] = -1;
        colors[*node] = WHITE;
    }
    ancestors[this->start_node] = this->start_node;
    /* add the start city's interest to current interest and to current itinerary */
    int start_interest = this->best_interest = this->cities[start_node].interest;
    this->best_distance = 0;
    PathElem tmp;
    tmp.city = this->start_node;
    list <PathElem> current;
    current.push_back (tmp);
    /* visit the start city */
    this->visit (this->start_node, colors, ancestors, this->best_distance, start_interest, current);

    delete[] (ancestors);
}

double
Gps::shortest_path (NodeId from, NodeId to)
{
    /* Adapted from Dijkstra, see rapport */
    NodeId *ancestors = new NodeId[this->nodes_count];
    double *deltas = new double[this->nodes_count];
    queue <NodeId> nexts; // We don't use a priority queue here since we do not ponderate nodes for now.
    map <NodeId, NodeColor> colors; // To track which nodes to explore
    for (NodeIdIter node = this->nodes_list.begin (), node_end = this->nodes_list.end (); node != node_end; ++node)
    {
        deltas[*node] = -1; /* -1 means infinity */
        ancestors[*node] = -1;
        colors[*node] = WHITE;
    }
    deltas[from] = 0;
    ancestors[from] = from;
    nexts.push (from);
    colors[from] = BLACK;
    /* There are still nodes to deal with */
    while (!nexts.empty ())
    {
        NodeId node = nexts.front ();
        nexts.pop ();
        ArcIds arcs = this->graph->list_arcs_from (node);
        /* Go to the next cities */
        for (ArcIdIter arc = arcs.begin (), arc_end = arcs.end (); arc != arc_end; ++arc)
        {
            /* Add the city to deal with if we didn't go there yet */
            ArcId arc_destination = this->graph->get_arc_details(*arc).second;
            if (colors[arc_destination] == WHITE)
            {
                nexts.push (arc_destination);
                colors[arc_destination] = BLACK;
            }
            /* if we just found a shorter path, register it */
            double distance = this->roads[*arc].length;
            /* -1 means infinity */
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
