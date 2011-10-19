#ifndef __LISTGRAPH_HH__
#define __LISTGRAPH_HH__

#include <list>
#include "../graph.hh"



class ListGraph : public Graph
{
	public:
		ListGraph();
		virtual int add_node () ;
		virtual void delete_node (int id) ;
		virtual int add_arc (int from, int to) ;
		virtual void remove_arc (int id) ;
		virtual set<int> list_successors (int id) ;
		virtual set<int> list_ancestors (int id) ;
		virtual set<int> list_arcs_from (int id) ;
		virtual set<int> list_arcs() ;
		virtual set<int> list_nodes() ;
		virtual set<int> list_arcs_to (int id) ;
		virtual int node_count () ;
		virtual int arcs_count () ;

	private:
	
		class ListGraphItem
		{
			public:
				ListGraphItem(int i){id=i;};
				int id;//pas besoin => iterateur
				std::list<int> listLinkedArcs;
		};
		std::list<ListGraphItem> list_links;
		

};

#endif /* LISTGRAPH_HH */ 
