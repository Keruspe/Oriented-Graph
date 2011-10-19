#include "ListGraph.hh"

ListGraph::ListGraph() : Graph()
{
	
}

int ListGraph::add_node () 
{
	int r = list_links.size();
	list_links.push_back(ListGraphItem(r));
	return r;
}

void ListGraph::delete_node (int id) 
{
	bool found = false;

	std::list<ListGraphItem>::iterator it=list_links.begin();
	
	while(found == false && it != list_links.end())
	{
		if(it->id == id)
		{
			list_links.erase(it);
			found = true;
		}
	}
}

int ListGraph::add_arc (int from, int to)
{
	bool found = false;

	std::list<ListGraphItem>::iterator it=list_links.begin();
	
	while(found == false && it != list_links.end())
	{
		if(it->id == to)
		{
			it->listLinkedArcs->push_back(from);
			found = true;
		}
	}
}

void ListGraph::remove_arc (int id)
{
	//probleme de O(N*M)? (N=list_links.size() && M = listLinkedArcs.size())
}

set<int> ListGraph::list_successors (int id)
{
}

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


