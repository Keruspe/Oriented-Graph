#include "ListGraph.hh"

ListGraph::ListGraph() : Graph()
{
	
}

int ListGraph::add_node () 
{
	list_links.push_back(ListGraphItem(list_links.size()));
}

void ListGraph::delete_node (int id) 
{
	bool trouve = false;

	std::list<ListGraphItem>::iterator it=list_links.begin();
	
	while(trouve == false && it != list_links.end())
	{
		if(it->id == id)
			list_links.erase(it);
	}
}

int ListGraph::add_arc (int from, int to)
{}

void ListGraph::remove_arc (int id)
{}

set<int> ListGraph::list_successors (int id)
{}

set<int> ListGraph::list_ancestors (int id)
{}

set<int> ListGraph::list_arcs_from (int id)
{}

set<int> ListGraph::list_arcs()
{}

set<int> ListGraph::list_nodes()
{}

set<int> ListGraph::list_arcs_to (int id)
{}

int ListGraph::node_count () 
{}

int ListGraph::arcs_count () 
{}


