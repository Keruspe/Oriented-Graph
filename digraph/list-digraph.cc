#include "list-digraph.hh"

ListDiGraph::ListDiGraph()
{
	current_node_id = 1;
	current_arc_id = 1;
	nodes_nb = 0;
	arcs_nb = 0;
}

ListDiGraph::~ListDiGraph() //O(n+m)
{
	listNodes.clear();
	listArcs.clear();
}

unsigned int ListDiGraph::add_node () //O(1)
{
	//@debug
	unsigned tempId = get_new_node_id ();
	
	std::cout<< "tempId="<<tempId <<std::endl;
	
	listNodes.push_back(Node(tempId));
	
	std::cout<< "listNodes assignation ok"<<listNodes.size() <<std::endl;
	
	nodes_nb++;
	
	return tempId;
}

void ListDiGraph::remove_node (unsigned Nid) 
{	
	listNodes.erase(listNodes.begin() + Nid);

	//Remove now useless arcs

	for(std::vector<Arc>::iterator it= listArcs.begin();it!=listArcs.end();it++)
	{
		if(it->from==Nid || it->to==Nid)
		{
			remove_arc(it->id);
		}
	}
	nodes_nb--;
}

unsigned int ListDiGraph::add_arc (unsigned NidFrom, unsigned NidTo) //O(1) ok
{
	unsigned tempId = get_new_arc_id (NidFrom, NidTo);
	get_node(NidFrom)->successors.push_back(NidTo);
	std::list<unsigned>::iterator it = get_node(NidFrom)->successors.end();
	it--;
	listArcs.push_back(Arc(tempId,NidFrom,NidTo,it));
		
	arcs_nb++;
	
	return tempId;
}

void ListDiGraph::remove_arc (unsigned Aid) //O(1) ok
{
	get_node(get_arc(Aid)->from)->successors.erase(get_arc(Aid)->itFrom);
	listArcs.erase(listArcs.begin() + Aid);

	arcs_nb--;
}

std::list<unsigned> ListDiGraph::list_successors (unsigned Nid) //O(1) ok
{
	return listNodes.at(Nid).successors;
}

std::list<unsigned> ListDiGraph::list_ancestors (unsigned Nid)
{
	std::list<unsigned> r;

	for(std::vector<Arc>::iterator it= listArcs.begin();it!=listArcs.end();it++)
	{
		if((*it).to==Nid)
			r.push_back((*it).from);
	}
	return r;
}

std::list<unsigned> ListDiGraph::list_nodes ()
{
    std::list<unsigned> lN;
	for(std::vector<Node>::iterator it= listNodes.begin();it!=listNodes.end();it++)
	{
		lN.push_back((*it).id);
	}
    return lN;
}

std::list<unsigned> ListDiGraph::list_arcs_from (unsigned Nid) //O(m)
{
	//@todo: détecter doublons
	std::list<unsigned> lA;
	for(std::vector<Arc>::iterator it= listArcs.begin();it!=listArcs.end();it++)
	{
		if((*it).from==Nid)
			lA.push_back((*it).id);
	}
	return lA;
}

std::list<unsigned> ListDiGraph::list_arcs_to (unsigned Nid) //O(m)
{
	std::list<unsigned> lA;
	for(std::vector<Arc>::iterator it= listArcs.begin();it!=listArcs.end();it++)
	{
		if((*it).to==Nid)
			lA.push_back((*it).id);
	}
	return lA;
}

std::list<unsigned> ListDiGraph::list_arcs_from_to (unsigned NidFrom, unsigned NidTo)
{
	std::list<unsigned> lA;
	for(std::vector<Arc>::iterator it= listArcs.begin();it!=listArcs.end();it++)
	{
		if((*it).from==NidFrom && (*it).to==NidTo)
			lA.push_back((*it).id);
	}
	return lA;
    return std::list<unsigned> ();
}

unsigned int ListDiGraph::nodes_count () //O(1)
{
	return nodes_nb;
}

unsigned int ListDiGraph::arcs_count () //O(1)
{
	return arcs_nb;
}

