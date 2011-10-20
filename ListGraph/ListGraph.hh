#ifndef __LISTGRAPH_HH__
#define __LISTGRAPH_HH__

#include <list>
#include "../graph.hh"



class ListGraph : public Graph
{
	public:
		ListGraph();
		virtual unsigned int add_node () ;
		virtual void delete_node (unsigned int id) ;
		virtual unsigned int add_arc (unsigned int from, unsigned int to) ;
		virtual void remove_arc (unsigned int id) ;
		virtual set<unsigned int> list_successors (unsigned int id) ;
		virtual set<unsigned int> list_ancestors (unsigned int id) ;
		virtual set<unsigned int> list_arcs_from (unsigned int id) ;
		virtual set<unsigned int> list_arcs() ;
		virtual set<unsigned int> list_nodes() ;
		virtual set<unsigned int> list_arcs_to (unsigned int id) ;
		virtual unsigned int node_count () ;
		virtual unsigned int arcs_count () ;

	private:
	
		class ListGraphItem
		{
			public:
				ListGraphItem(unsigned int i){id=i;};
				unsigned int id;//pas besoin => iterateur
				std::list<unsigned int> listLinkedArcs;
		};
		std::list<ListGraphItem> list_links;
		

};

#endif /* LISTGRAPH_HH */ 
