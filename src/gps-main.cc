#include "digraph/array-digraph.hh"
#include "digraph/list-digraph.hh"
#include "gps.hh"

#include <iostream>
#include <sys/time.h>

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
    double coeff;
    switch (method) {
    case BOUNDED_DETOUR:
        cout << "Value for K ?" << endl;
        cin >> coeff;
        break;
    case AGREGATION:
        cout << "Value for A ?" << endl;
        cin >> coeff;
        break;
    }
    string path;
    cout << "Which file contains the map description ?" << endl;
    cin >> path;
    string start;
    cout << "Where do you wanna leave ?" << endl;
    cin >> start;
    string dest;
    cout << "Where do you wanna go ?" << endl;
    cin >> dest;
    Gps gps (graph, path, coeff, start, dest);
    struct timeval start_time, end_time;
    gettimeofday (&start_time, NULL);
    switch (method) {
    case AGREGATION:
        gps.calculate_by_agregation ();
        break;
    case BOUNDED_DETOUR:
        gps.calculate_by_bounded_detour ();
        break;
    }
    gettimeofday (&end_time, NULL);
    cout << endl
        << "Took " << end_time.tv_usec - start_time.tv_usec << "µs to calculate" << endl;
    delete (graph);
    return 0;
}
