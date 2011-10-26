#include "array-graph/array-graph.hh"

#include <iostream>

using std::cout;
using std::endl;

void
print_set (const ArcIds &ids)
{
    for (ArcIdIter i = ids.begin (), i_end = ids.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl << endl;
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

    cout << "Nodes:" << endl
         << "Expected: " << a << " " << b << " " << c << " " << d << " " << e << " " << f << endl
         << "Got:      ";
    print_set (g.list_nodes ());

    cout << "Arcs:" << endl
         << "Expected: " << aa << " " << ab << " " << ac << " " << ad << " " << cd << " " << de << " " << ef << " " << fd << endl
         << "Got:      ";
    print_set (g.list_arcs ());

    cout << "Nodes from a:" << endl
         << "Expected: " << a << " " << b << " " << c << " " << d << endl
         << "Got:      ";
    print_set (g.list_successors (a));

    cout << "Nodes to d:" << endl
         << "Expected: " << a << " " << c << " " << f << endl
         << "Got:      ";
    print_set (g.list_ancestors (d));

    cout << "Arcs from a:" << endl
         << "Expected: " << aa << " " << ab << " " << ac << " " << ad << endl
         << "Got:      ";
    print_set (g.list_arcs_from (a));

    cout << "Arcs to d:" << endl
         << "Expected: " << ad << " " << cd << " " << fd << endl
         << "Got:      ";
    print_set (g.list_arcs_to (d));

    cout << "--- ab arc removal ---" << endl << endl;
    g.remove_arc (ab);

    cout << "Arcs:" << endl
         << "Expected: " << aa << " " << ac << " " << ad << " " << cd << " " << de << " " << ef << " " << fd << endl
         << "Got:      ";
    print_set (g.list_arcs ());

    cout << "Arcs from a:" << endl
         << "Expected: " << aa << " " << ac << " " << ad << endl
         << "Got:      ";
    print_set (g.list_arcs_from (a));

    cout << "Nodes from a:" << endl
         << "Expected: " << a << " " << c << " " << d << endl
         << "Got:      ";
    print_set (g.list_successors (a));

    cout << "--- c node removal ---" << endl << endl;
    g.delete_node (c);

    cout << "Nodes:" << endl
         << "Expected: " << a << " " << b << " " << d << " " << e << " " << f << endl
         << "Got:      ";
    print_set (g.list_nodes ());

    cout << "Nodes from a:" << endl
         << "Expected: " << a << " " << d << endl
         << "Got:      ";
    print_set (g.list_successors (a));

    cout << "Nodes to d:" << endl
         << "Expected: " << a << " " << f << endl
         << "Got:      ";
    print_set (g.list_ancestors (d));

    cout << "Nodes count:" << endl
         << "Expected: 5" << endl 
         << "Got:      " << g.nodes_count () << endl << endl;

    cout << "Arcs count:" << endl
         << "Expected: 5" << endl
         << "Got:      " << g.arcs_count () << endl;

    return 0;
}
