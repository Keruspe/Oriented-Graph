#include "array-graph.hh"

#include <queue>

using std::queue;

ArrayGraph::ArrayGraph () :
    Graph (),
    _nodes_count (0),
    _arcs_count (0)
{
}

ArrayGraph::~ArrayGraph ()
{
}

NodeId
ArrayGraph::add_node ()
{
    NodeId id = this->get_new_node_id ();
    this->node_exists[id] = true;
    this->matrix.push_back (EndNodes (this->_nodes_count));
    for (StartNodeIter i = this->matrix.begin (), i_end = this->matrix.end (); i != i_end; ++i)
        i->push_back (ArcIds ());
    ++this->_nodes_count;
    return id;
}

void
ArrayGraph::remove_node (NodeId id)
{
    if (!this->node_exists[id])
        return;
    this->node_exists[id] = false;
    EndNodes &node = this->matrix[id];
    for (EndNodeIter i = node.begin (), i_end = node.end (); i != i_end; ++i)
        this->_arcs_count -= i->size ();
    for (StartNodeIter i = this->matrix.begin (), i_end = this->matrix.end (); i != i_end; ++i)
        this->_arcs_count -= (*i)[id].size ();
    --this->_nodes_count;
}

ArcId
ArrayGraph::add_arc (NodeId from, NodeId to)
{
    ArcId id = this->get_new_arc_id ();
    this->matrix[from][to].push_back (id);
    this->arcs[id] = std::make_pair <NodeId, NodeId> (from, to);
    ++this->_arcs_count;
    return id;
}

void
ArrayGraph::remove_arc (ArcId id)
{
    pair <NodeId, NodeId> &nodes = this->arcs[id];
    ArcIds &tmp = this->matrix[nodes.first][nodes.second];
    for (ArcIdIter i = tmp.begin (), i_end = tmp.end (); i != i_end; ++i)
    {
        if (*i != id)
            continue;
        tmp.erase (i);
        --this->_arcs_count;
        break;
    }
}

NodeIds
ArrayGraph::list_successors (NodeId id)
{
    NodeIds successors;
    if (!this->node_exists[id])
        return successors;

    EndNodes &node = this->matrix[id];
    NodeId tmp = 0;
    for (EndNodeIter i = node.begin (), i_end = node.end (); i != i_end; ++i, ++tmp)
    {
        if (!this->node_exists[tmp])
            continue;
        if (!i->empty ())
            successors.push_back (tmp);
    }
    return successors;
}

NodeIds
ArrayGraph::list_ancestors (NodeId id)
{
    NodeIds ancestors;
    if (!this->node_exists[id])
        return ancestors;

    NodeId tmp = 0;
    for (StartNodeIter i = this->matrix.begin (), i_end = this->matrix.end (); i != i_end; ++i, ++tmp)
    {
        if (!this->node_exists[tmp])
            continue;
        if (!(*i)[id].empty ())
            ancestors.push_back (tmp);
    }
    return ancestors;
}

NodeIds
ArrayGraph::list_nodes ()
{
    NodeIds nodes;
    for (NodeId i = 0; i < this->next_node_id (); ++i)
    {
        if (this->node_exists[i])
            nodes.push_back (i);
    }
    return nodes;
}

ArcIds
ArrayGraph::list_arcs_from (NodeId id)
{
    ArcIds arcs_from;
    if (!this->node_exists[id])
        return arcs_from;

    EndNodes &node = this->matrix[id];
    NodeId tmp = 0;
    for (EndNodeIter i = node.begin (), i_end = node.end (); i != i_end; ++i, ++tmp)
    {
        if (!this->node_exists[tmp])
            continue;
        for (ArcIdIter j = i->begin (), j_end = i->end (); j != j_end; ++j)
            arcs_from.push_back (*j);
    }
    return arcs_from;
}

ArcIds
ArrayGraph::list_arcs_to (NodeId id)
{
    ArcIds arcs_to;
    if (!this->node_exists[id])
        return arcs_to;

    NodeId tmp = 0;
    for (StartNodeIter i = this->matrix.begin (), i_end = this->matrix.end (); i != i_end; ++i, ++tmp)
    {
        if (!this->node_exists[tmp])
            continue;
        ArcIds &tmp = (*i)[id];
        for (ArcIdIter j = tmp.begin (), j_end = tmp.end (); j != j_end; ++j)
            arcs_to.push_back (*j);
    }
    return arcs_to;
}

ArcIds
ArrayGraph::list_arcs_from_to (NodeId from, NodeId to)
{
    if (!this->node_exists[from] || !this->node_exists[to])
        return ArcIds ();
    return this->matrix[from][to];
}

ostream &
operator<< (ostream &os, ArrayGraph &graph)
{
    os << " ";
    NodeId node_id = 0;
    for (StartNodeIter i = graph.matrix.begin (), i_end = graph.matrix.end (); i != i_end; ++i, ++node_id)
    {
        if (!graph.node_exists[node_id])
            continue;
        os << "   " << node_id;
    }
    os << endl;
    node_id = 0;
    for (StartNodeIter i = graph.matrix.begin (), i_end = graph.matrix.end (); i != i_end; ++i, ++node_id)
    {
        if (!graph.node_exists[node_id])
            continue;
        os << node_id;
        NodeId ni = 0;
        for (EndNodeIter j = i->begin (), j_end = i->end (); j != j_end; ++j, ++ni)
        {
            if (!graph.node_exists[ni])
                continue;
            os << "   " << j->size ();
        }
        os << endl;
    }
    return os;
}
