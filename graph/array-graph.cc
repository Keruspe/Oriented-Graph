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
    for (StartNodeIter from = this->matrix.begin (), from_end = this->matrix.end (); from != from_end; ++from)
        from->push_back (ArcIds ());
    ++this->_nodes_count;
    return id;
}

void
ArrayGraph::remove_node (NodeId id)
{
    if (!this->node_exists[id])
        return;
    this->node_exists[id] = false;
    EndNodes &nodes = this->matrix[id];
    for (EndNodeIter to = nodes.begin (), to_end = nodes.end (); to != to_end; ++to)
        this->_arcs_count -= to->size ();
    for (StartNodeIter from = this->matrix.begin (), from_end = this->matrix.end (); from != from_end; ++from)
        this->_arcs_count -= (*from)[id].size ();
    --this->_nodes_count;
}

ArcId
ArrayGraph::add_arc (NodeId from, NodeId to)
{
    ArcId id = this->get_new_arc_id (from, to);
    this->matrix[from][to].push_back (id);
    ++this->_arcs_count;
    return id;
}

void
ArrayGraph::remove_arc (ArcId id)
{
    pair <NodeId, NodeId> &nodes = this->arcs[id];
    ArcIds &arcs = this->matrix[nodes.first][nodes.second];
    for (ArcIdIter arc = arcs.begin (), arc_end = arcs.end (); arc != arc_end; ++arc)
    {
        if (*arc == id)
        {
            arcs.erase (arc);
            --this->_arcs_count;
            break;
        }
    }
}

NodeIds
ArrayGraph::list_successors (NodeId id)
{
    NodeIds successors;
    if (!this->node_exists[id])
        return successors;

    EndNodes &nodes = this->matrix[id];
    unsigned int index = 0;
    EndNodeIter start_iter = nodes.begin ();
    for (EndNodeIter to = start_iter, to_end = nodes.end (); to != to_end; to = start_iter + ++index)
    {
        if (!this->node_exists[index])
            continue;
        if (!to->empty ())
            successors.push_back (index);
    }
    return successors;
}

NodeIds
ArrayGraph::list_ancestors (NodeId id)
{
    NodeIds ancestors;
    if (!this->node_exists[id])
        return ancestors;

    unsigned int index = 0;
    StartNodeIter start_iter = this->matrix.begin ();
    for (StartNodeIter from = start_iter, from_end = this->matrix.end (); from != from_end; from = start_iter + ++index)
    {
        if (!this->node_exists[index])
            continue;
        if (!(*from)[id].empty ())
            ancestors.push_back (index);
    }
    return ancestors;
}

NodeIds
ArrayGraph::list_nodes ()
{
    NodeIds nodes;
    unsigned int index = 0;
    StartNodeIter start_iter = this->matrix.begin ();
    for (StartNodeIter from = start_iter, from_end = this->matrix.end (); from != from_end; from = start_iter + ++index)
    {
        if (this->node_exists[index])
            nodes.push_back (index);
    }
    return nodes;
}

ArcIds
ArrayGraph::list_arcs_from (NodeId id)
{
    ArcIds arcs_from;
    if (!this->node_exists[id])
        return arcs_from;

    EndNodes &nodes = this->matrix[id];
    unsigned int index = 0;
    EndNodeIter start_iter = nodes.begin ();
    for (EndNodeIter to = start_iter, to_end = nodes.end (); to != to_end; to = start_iter + ++index)
    {
        if (!this->node_exists[index])
            continue;
        for (ArcIdIter arc = to->begin (), arc_end = to->end (); arc != arc_end; ++arc)
            arcs_from.push_back (*arc);
    }
    return arcs_from;
}

ArcIds
ArrayGraph::list_arcs_to (NodeId id)
{
    ArcIds arcs_to;
    if (!this->node_exists[id])
        return arcs_to;

    unsigned int index = 0;
    StartNodeIter start_iter = this->matrix.begin ();
    for (StartNodeIter from = start_iter, from_end = this->matrix.end (); from != from_end; from = start_iter + ++index)
    {
        if (!this->node_exists[index])
            continue;
        ArcIds &arcs = (*from)[id];
        for (ArcIdIter arc = arcs.begin (), arc_end = arcs.end (); arc != arc_end; ++arc)
            arcs_to.push_back (*arc);
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
