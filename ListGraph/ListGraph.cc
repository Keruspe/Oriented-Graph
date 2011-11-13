#include "ListGraph.hh"

ListGraph::ListGraph()
{
	current_node_id = 1;
	current_arc_id = 1;
	nodes_nb = 0;
	arcs_nb = 0;
}

ListGraph::~ListGraph() //O(n+m)
{
	listNodes.clear();
	listArcs.clear();
}

unsigned int ListGraph::add_node () //O(1)
{
	unsigned tempId = get_next_node_id ();
	listNodes.at(tempId) = Node(tempId);
	
	nodes_nb++;
	
	return tempId;
}

void ListGraph::delete_node (unsigned Nid) 
{	
	listNodes.erase(listNodes.begin() + Nid);
	for(std::vector<Arc>::iterator it= listArcs.begin();it!=listArcs.end();it++)
	{
		if(get_node((*it).from)->id == Nid)
			get_node((*it).from)->successors.erase((*it).itFrom);
	}
	
	nodes_nb--;	
}

unsigned int ListGraph::add_arc (unsigned NidFrom, unsigned NidTo) //O(1) ok
{
	unsigned tempId = get_next_arc_id ();
	get_node(NidFrom)->successors.push_back(NidTo);
	listArcs.at(current_arc_id) = Arc(tempId,NidFrom,NidTo);
	
	arcs_nb++;
	
	return tempId;
}

void ListGraph::remove_arc (unsigned Aid) //O(1) ok
{
	get_node(get_arc(Aid)->from)->successors.erase(get_arc(Aid)->itFrom);
	listArcs.erase(listArcs.begin() + Aid);

	arcs_nb--;
}

std::list<unsigned> ListGraph::list_successors (unsigned Nid) //O(1) ok
{
	return listNodes.at(Nid).successors;
}

std::list<unsigned> ListGraph::list_ancestors (unsigned Nid)
{
	std::list<unsigned> r;
	for(std::vector<Arc>::iterator it= listArcs.begin();it!=listArcs.end();it++)
	{
		//TODO: check if it's (*it).from instead of (*it).to
		if((*it).from==Nid)
			r.push_back((*it).from);
	}
	return r;
}

std::list<unsigned> ListGraph::list_arcs_from (unsigned Nid) //O(m)
{
	std::list<unsigned> lA;
	for(std::vector<Arc>::iterator it= listArcs.begin();it!=listArcs.end();it++)
	{
		if((*it).from==Nid)
			lA.push_back((*it).id);
	}
	return lA;
}

std::list<unsigned> ListGraph::list_arcs_to (unsigned Nid) //O(m)
{
	std::list<unsigned> lA;
	for(std::vector<Arc>::iterator it= listArcs.begin();it!=listArcs.end();it++)
	{
		if((*it).to==Nid)
			lA.push_back((*it).id);
	}
	return lA;
}

unsigned int ListGraph::node_count () //O(1)
{
	return nodes_nb;
}

unsigned int ListGraph::arcs_count () //O(1)
{
	return arcs_nb;
}

//======================================================================

//~ std::vector<Node>::iterator ListGraph::getNodeIteratorById(unsigned int i)
//~ {
	//~ bool found = false;
	//~ std::vector<Node>::iterator it = listNodes.begin();
	//~ 
	//~ while(found == false && it!=listNodes.end())
	//~ {
		//~ if(it->id == i)
			//~ found = true;
		//~ it++;
	//~ }
	//~ if(found==true)
		//~ return it;
//~ }
//~ 
