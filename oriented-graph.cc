#include "array-graph/array-graph.hh"

#include <iostream>

using std::cout;
using std::endl;

int
main()
{
    ArrayGraph g;
    unsigned int a = g.add_node ();
    unsigned int b = g.add_node ();
    unsigned int c = g.add_node ();
    unsigned int d = g.add_node ();
    unsigned int e = g.add_node ();
    unsigned int f = g.add_node ();
    unsigned int aa = g.add_arc (a, a);
    unsigned int ab = g.add_arc (a, b);
    unsigned int ac = g.add_arc (a, c);
    unsigned int ad = g.add_arc (a, d);
    unsigned int cd = g.add_arc (c, d);
    unsigned int de = g.add_arc (d, e);
    unsigned int ef = g.add_arc (e, f);
    unsigned int fd = g.add_arc (f, d);
    cout << "Nodes: (Expected " << a << " " << b << " " << c << " " << d << " " << e << " " << f << ")" << endl;
    set <unsigned int> tmp = g.list_nodes ();
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Arcs: (Expected " << aa << " " << ab << " " << ac << " " << ad << " " << cd << " " << de << " " << ef << " " << fd << ")" << endl;
    tmp = g.list_arcs ();
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Nodes from a: (Expected " << a << " " << b << " " << c << " " << d << ")" << endl;
    tmp = g.list_successors (a);
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Nodes to d: (Expected " << a << " " << c << " " << f << ")" << endl;
    tmp = g.list_ancestors (d);
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Arcs from a: (Expected " << aa << " " << ab << " " << ac << " " << ad << ")" << endl;
    tmp = g.list_arcs_from (a);
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Arcs to d: (Expected " << ad << " " << cd << " " << fd << ")" << endl;
    tmp = g.list_arcs_to (d);
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Remove the ab arc" << endl;
    g.remove_arc (ab);
    cout << "Arcs: (Expected " << aa << " " << ac << " " << ad << " " << cd << " " << de << " " << ef << " " << fd << ")" << endl;
    tmp = g.list_arcs ();
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Arcs from a: (Expected " << aa << " " << ac << " " << ad << ")" << endl;
    tmp = g.list_arcs_from (a);
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Nodes from a: (Expected " << a << " " << c << " " << d << ")" << endl;
    tmp = g.list_successors (a);
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Remove the c node" << endl;
    g.delete_node (c);
    cout << "Nodes: (Expected " << a << " " << b << " " << d << " " << e << " " << f << ")" << endl;
    tmp = g.list_nodes ();
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Nodes from a: (Expected " << a << " " << d << ")" << endl;
    tmp = g.list_successors (a);
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Nodes to d: (Expected " << a << " " << f << ")" << endl;
    tmp = g.list_ancestors (d);
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Nodes count: (Expected 5)" << endl << g.nodes_count () << endl;
    cout << "Arcs count: (Expected 5)" << endl << g.arcs_count () << endl;
    return 0;
}
