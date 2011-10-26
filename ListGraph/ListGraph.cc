#include "ListGraph.hh"

ListGraph::ListGraph() : Graph()
{
	
}

unsigned int ListGraph::add_node () 
{
	unsigned int r = get_new_node_id ();
	list_links.push_back(ListGraphItem(r));
	return r;
}

void ListGraph::delete_node (unsigned int id) 
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

unsigned int ListGraph::add_arc (unsigned int from, unsigned int to)
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

        // TODO: return id   (get_new_arc_id ())
}

void ListGraph::remove_arc (unsigned int id)
{
	//probleme de O(N*M)? (N=list_links.size() && M = listLinkedArcs.size())
}

set<unsigned int> ListGraph::list_successors (unsigned int id)
{
}

set<unsigned int> ListGraph::list_ancestors (unsigned int id)
{}

set<unsigned int> ListGraph::list_arcs_from (unsigned int id)
{}

set<unsigned int> ListGraph::list_arcs()
{}

set<unsigned int> ListGraph::list_nodes()
{}

set<unsigned int> ListGraph::list_arcs_to (unsigned int id)
{}

unsigned int ListGraph::node_count () 
{}

unsigned int ListGraph::arcs_count () 
{}


