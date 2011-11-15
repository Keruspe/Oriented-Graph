#include "digraph/array-digraph.hh"
#include "digraph/list-digraph.hh"

void
print_ids (ConstIds &ids)
{
    for (ConstIdIter i = ids.begin (), i_end = ids.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl << endl;
}

int
main()
{
    DiGraph *g;
    //g = new ListDiGraph ();
    g = new ArrayDiGraph ();

    NodeId a = g->add_node ();
    NodeId b = g->add_node ();
    NodeId c = g->add_node ();
    NodeId d = g->add_node ();
    NodeId e = g->add_node ();
    NodeId f = g->add_node ();

    ArcId bb = g->add_arc (b, b);
    ArcId bb_bis = g->add_arc (b, b);
    ArcId bc = g->add_arc (b, c);
    ArcId bd = g->add_arc (b, d);
    ArcId be = g->add_arc (b, e);
    ArcId de = g->add_arc (d, e);
    g->add_arc (e, a);
    g->add_arc (a, f);
    ArcId fe = g->add_arc (f, e);

    cout << "Graph as matrix:" << endl << g->as_matrix () << endl;

    cout << "Graph as list:" << endl << g->as_list () << endl;

    cout << "Connexity:" << endl
         << "Expected: yes" << endl
         << "Got:      " << (g->connex () ? "yes" : "no") << endl << endl;

    cout << "Acyclicity:" << endl
         << "Expected: no" << endl
         << "Got:      " << (g->acyclic () ? "yes" : "no") << endl << endl;

    cout << "Simplicity:" << endl
         << "Expected: no" << endl
         << "Got:      " << (g->simple () ? "yes" : "no") << endl << endl;

    cout << "Nodes:" << endl
         << "Expected: " << a << " " << b << " " << c << " " << d << " " << e << " " << f << endl
         << "Got:      ";
    print_ids (g->list_nodes ());

    cout << "Nodes from b:" << endl
         << "Expected: " << b << " " << c << " " << d << " " << e << endl
         << "Got:      ";
    print_ids (g->list_successors (b));

    cout << "Nodes to e:" << endl
         << "Expected: " << b << " " << d << " " << f << endl
         << "Got:      ";
    print_ids (g->list_ancestors (e));

    cout << "Arcs from b:" << endl
         << "Expected: " << bb << " " << bb_bis << " " << bc << " " << bd << " " << be << endl
         << "Got:      ";
    print_ids (g->list_arcs_from (b));

    cout << "Arcs to e:" << endl
         << "Expected: " << be << " " << de << " " << fe << endl
         << "Got:      ";
    print_ids (g->list_arcs_to (e));

    cout << "--- Depth-first search ---" << endl << endl;
    g->depth_first_search (a, true);
    cout << endl;

    cout << "--- Complete depth-first search ---" << endl << endl;
    g->depth_first_search (-1, true);
    cout << endl;

    cout << "--- Breadth-first search ---" << endl << endl;
    g->breadth_first_search (a, true);
    cout << endl;

    cout << "--- Complete breadth-first search ---" << endl << endl;
    g->breadth_first_search (-1, true);
    cout << endl;

    cout << "--- bc arc removal ---" << endl << endl;
    g->remove_arc (bc);

    cout << "Arcs from b:" << endl
         << "Expected: " << bb << " " << bb_bis << " " << bd << " " << be << endl
         << "Got:      ";
    print_ids (g->list_arcs_from (b));

    cout << "Nodes from b:" << endl
         << "Expected: " << b << " " << d << " " << e << endl
         << "Got:      ";
    print_ids (g->list_successors (b));

    cout << "--- d node removal ---" << endl << endl;
    g->remove_node (d);

    cout << "Nodes:" << endl
         << "Expected: " << a << " " << b << " " << c << " " << e << " " << f << endl
         << "Got:      ";
    print_ids (g->list_nodes ());

    cout << "Nodes from b:" << endl
         << "Expected: " << b << " " << e << endl
         << "Got:      ";
    print_ids (g->list_successors (b));

    cout << "Nodes to e:" << endl
         << "Expected: " << b << " " << f << endl
         << "Got:      ";
    print_ids (g->list_ancestors (e));

    cout << "Nodes count:" << endl
         << "Expected: 5" << endl 
         << "Got:      " << g->nodes_count () << endl << endl;

    cout << "Arcs count:" << endl
         << "Expected: 6" << endl
         << "Got:      " << g->arcs_count () << endl << endl;

    delete (g);

    return 0;
}
