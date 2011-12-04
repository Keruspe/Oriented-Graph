#include <iostream>

#include "digraph/array-digraph.hh"
#include "digraph/list-digraph.hh"
#include "gps.hh"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

typedef enum {
    AGREGATION,
    BOUNDED_DETOUR
} Method;

int
main ()
{
    int answer;
    cout << "Which implementation of DiGraph would you prefer ?" << endl
        << "1. ArrayDiGraph (Adjacency matrix)" << endl
        << "2. ListDiGraph (Adjacency list)" << endl;
    cin >> answer;
    DiGraph *graph;
    switch (answer)
    {
    case 1:
        graph = new ArrayDiGraph ();
        break;
    case 2:
        graph = new ListDiGraph ();
        break;
    default:
        cerr << "Bad choice: " << answer << endl;
        return -1;
    }
    cout << "Which method would you prefer to calculate the intinerary ?" << endl
        << "1. Agregation method" << endl
        << "2. Bounded detour method" << endl;
    cin >> answer;
    Method method;
    switch (answer)
    {
    case 1:
        method = AGREGATION;
        break;
    case 2:
        method = BOUNDED_DETOUR;
        break;
    default:
        cerr << "Bad choice: " << answer << endl;
        return -1;
    }
    double K, A;
    switch (method) {
    case BOUNDED_DETOUR:
        cout << "Value for K ?" << endl;
        cin >> K;
        break;
    case AGREGATION:
        cout << "Value for A ?" << endl;
        cin >> A;
        break;
    }
    string path;
    cout << "Which file contains the map description ?" << endl;
    cin >> path;
    Gps gps (graph, path, K, A);
    string start;
    cout << "Where do you wanna leave ?" << endl;
    cin >> start;
    string dest;
    cout << "Where do you wanna go ?" << endl;
    cin >> dest;
    switch (method) {
    case AGREGATION:
        gps.calculate_by_agregation (start, dest);
        break;
    case BOUNDED_DETOUR:
        gps.calculate_by_bounded_detour (start, dest);
        break;
    }
    delete (graph);
    return 0;
}
