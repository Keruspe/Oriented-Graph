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
    unsigned int cd = g.add_arc (c, d);
    unsigned int ad = g.add_arc (a, d);
    unsigned int de = g.add_arc (d, e);
    unsigned int ef = g.add_arc (e, f);
    unsigned int fd = g.add_arc (f, d);
    cout << "Nodes from a: (Expected " << a << " " << b << " " << c << " " << d << ")" << endl;
    set <unsigned int> tmp = g.list_successors (a);
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    cout << "Nodes to d: (Expected " << c << " " << a << " " << f << ")" << endl;
    tmp = g.list_successors (a);
    for (set <unsigned int>::iterator i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
        cout << *i << " ";
    cout << endl;
    return 0;
}
