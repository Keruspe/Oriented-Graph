#include "gps.hh"
#include "map-parser.hh"


#include <vector>
#include <queue>

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

Gps::Gps (DiGraph *_graph, string path, int _K, int _A) :
    graph (_graph),
    K (_K),
    A (_A)
{
    MapParser parser(_graph, path, this->nodes, this->cities, this->roads, this->nodes_list, this->arcs_list, &this->dmax, &this->imax);
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
	std::cout<< "@debug1" <<std::endl;
	for (; it != nodes.end();it++)
	{
		std::cout<< "@debug4" <<std::endl;
		Elem e(0, (it->second));
		pq.push(e);
		std::cout<< "@debug6::"<< it->second <<std::endl;
		d[it->second]=999;
		std::cout<< "@debug5" <<std::endl;
	}
	
	std::cout<< "@debug2" <<std::endl;
	
	while(!pq.empty())
	{
		std::cout<< "@debug31" <<std::endl;
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
		std::cout<< "@debug32" <<std::endl;
	}

}

void
Gps::calculate_by_bounded_detour (string start, string dest)
{
    NodeId start_node = this->nodes[start];
    NodeId end_node = this->nodes[dest];
}
