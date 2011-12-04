#include "map-parser.hh"

#include <sstream>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

MapParser::MapParser (DiGraph *_graph, string path, map <string, NodeId> &nodes, map <NodeId, Gps::City> &cities, map <ArcId, Gps::Road> &roads, double *dmax, int *imax) :
    graph (_graph)
{
    ifstream ifs (path.c_str ());
    this->map_file = &ifs;
    if (!this->map_file->is_open())
        cerr << "Couldn't read file" << endl;
    else
    {
        if (this->expect ("digraph") && this->expect ("{"))
        {
            for (;;)
            {
                if (this->next_token () == "" || this->token == "}")
                    break;
                this->id_or_from = this->token;
                if (this->next_token () == "")
                    break;
                if (this->token == "->")
                {
                    this->dest = this->next_token ();
                    this->next_token ();
                }
                else
                    this->dest = "";
                this->handle_label ();
                if (this->interest > *imax)
                    *imax = this->interest;
                if (this->name == "")
                {
                    cout << "New city: " << this->id_or_from << ", with label: " << this->label << " and interest: " << this->interest << endl;
                    NodeId nid = graph->add_node ();
                    nodes[this->id_or_from] = nid;
                    Gps::City city;
                    city.label = this->label;
                    city.interest = this->interest;
                    cities[nid] = city;
                }
                else
                {
                    if (this->length > *dmax)
                        *dmax = this->length;
                    cout << "New road: " << this->name << " from " << this->id_or_from << " to " << this->dest << ", with label: " << this->label << ", interest: " << this->interest << " and length: " << this->length << endl;
                    ArcId aid = graph->add_arc (nodes[this->id_or_from], nodes[this->dest]);
                    nodes[this->name] = aid;
                    Gps::Road road;
                    road.label = this->label;
                    road.interest = this->interest;
                    road.length = this->length;
                    roads[aid] = road;
                }
            }
        }
        if (token != "}")
        {
            cerr << "Malformed file, expected } but got " << token << endl;
        }
        this->map_file->close();
    }
}

string
MapParser::next_token ()
{
    if (!this->map_file->good ())
    {
        cerr << "Unexpected EOF" << endl;
        return "";
    }
    *(this->map_file) >> this->token;
    return this->token;
}

bool
MapParser::expect (string expected)
{
    if (this->next_token () != expected)
    {
        cerr << "Malformed file, expected " << expected << " but got " << this->token << endl;
        return false;
    }
    return true;
}

static vector <string>
split (string str, char delim)
{
    istringstream iss (str);
    vector <string> splitted;
    string token;
    while (getline (iss, token, delim))
        splitted.push_back (token);
    return splitted;
}

void
MapParser::handle_label ()
{
    vector <string> splitted = split (this->token, '"');
    if (splitted.size () != 3)
    {
        this->label = "";
        return;
    }
    this->label = splitted[1];
    
    splitted = split (this->label, '(');
    if (splitted.size () != 2)
    {
        this->interest = 0;
        return;
    }

    string interest = splitted[1];
    size_t limit = interest.find (';');
    if (limit != string::npos)
    {
        this->name = splitted[0];
        istringstream iss (interest.substr (0, limit));
        iss >> this->length;
        interest = interest.substr (limit + 1);
    }
    this->interest = interest.length () - 1;
}
