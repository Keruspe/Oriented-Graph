#include "array-graph/array-graph.hh"

#include <iostream>

using std::cout;
using std::endl;

void
print_set (const set <ArcId> &ids)
{
    for (set <ArcId>::iterator i = ids.begin (), i_end = ids.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
}

int
main()
{
    ArrayGraph g;

    NodeId a = g.add_node ();
    NodeId b = g.add_node ();
    NodeId c = g.add_node ();
    NodeId d = g.add_node ();
    NodeId e = g.add_node ();
    NodeId f = g.add_node ();

    ArcId aa = g.add_arc (a, a);
    ArcId ab = g.add_arc (a, b);
    ArcId ac = g.add_arc (a, c);
    ArcId ad = g.add_arc (a, d);
    ArcId cd = g.add_arc (c, d);
    ArcId de = g.add_arc (d, e);
    ArcId ef = g.add_arc (e, f);
    ArcId fd = g.add_arc (f, d);

    cout << "Nodes: (Expected " << a << " " << b << " " << c << " " << d << " " << e << " " << f << ")" << endl;
    print_set (g.list_nodes ());

    cout << "Arcs: (Expected " << aa << " " << ab << " " << ac << " " << ad << " " << cd << " " << de << " " << ef << " " << fd << ")" << endl;
    print_set (g.list_arcs ());

    cout << "Nodes from a: (Expected " << a << " " << b << " " << c << " " << d << ")" << endl;
    print_set (g.list_successors (a));

    cout << "Nodes to d: (Expected " << a << " " << c << " " << f << ")" << endl;
    print_set (g.list_ancestors (d));

    cout << "Arcs from a: (Expected " << aa << " " << ab << " " << ac << " " << ad << ")" << endl;
    print_set (g.list_arcs_from (a));

    cout << "Arcs to d: (Expected " << ad << " " << cd << " " << fd << ")" << endl;
    print_set (g.list_arcs_to (d));

    cout << "Remove the ab arc" << endl;
    g.remove_arc (ab);

    cout << "Arcs: (Expected " << aa << " " << ac << " " << ad << " " << cd << " " << de << " " << ef << " " << fd << ")" << endl;
    print_set (g.list_arcs ());

    cout << "Arcs from a: (Expected " << aa << " " << ac << " " << ad << ")" << endl;
    print_set (g.list_arcs_from (a));

    cout << "Nodes from a: (Expected " << a << " " << c << " " << d << ")" << endl;
    print_set (g.list_successors (a));

    cout << "Remove the c node" << endl;
    g.delete_node (c);

    cout << "Nodes: (Expected " << a << " " << b << " " << d << " " << e << " " << f << ")" << endl;
    print_set (g.list_nodes ());

    cout << "Nodes from a: (Expected " << a << " " << d << ")" << endl;
    print_set (g.list_successors (a));

    cout << "Nodes to d: (Expected " << a << " " << f << ")" << endl;
    print_set (g.list_ancestors (d));

    cout << "Nodes count: (Expected 5)" << endl << g.nodes_count () << endl;

    cout << "Arcs count: (Expected 5)" << endl << g.arcs_count () << endl;

    return 0;
}
