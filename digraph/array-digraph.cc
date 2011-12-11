#include "array-digraph.hh"

#include <queue>

using std::queue;

ArrayDiGraph::ArrayDiGraph () :
    DiGraph (),
    _nodes_count (0),
    _arcs_count (0)
{
}

ArrayDiGraph::~ArrayDiGraph ()
{
}

NodeId
ArrayDiGraph::add_node ()
{
    NodeId id = this->get_new_node_id ();
    this->node_exists[id] = true;
    /* We add a new line to the matrix */
    this->matrix.push_back (EndNodes (this->_nodes_count));
    /* Then we add a new col to each line of the matrix */
    for (StartNodeIter from = this->matrix.begin (), from_end = this->matrix.end (); from != from_end; ++from)
        from->push_back (ArcIds ());
    ++this->_nodes_count;
    return id;
}

void
ArrayDiGraph::remove_node (NodeId id)
{
    if (!this->node_exists[id])
        return;
    this->node_exists[id] = false;
    EndNodes &nodes = this->matrix[id];
    /* We remove all the arcs going to the node from the arcs count */
    for (EndNodeIter to = nodes.begin (), to_end = nodes.end (); to != to_end; ++to)
        this->_arcs_count -= to->size ();
    /* We remove all the arcs leaving the node from the arcs count */
    for (StartNodeIter from = this->matrix.begin (), from_end = this->matrix.end (); from != from_end; ++from)
        this->_arcs_count -= (*from)[id].size ();
    --this->_nodes_count;
}

ArcId
ArrayDiGraph::add_arc (NodeId from, NodeId to)
{
    ArcId id = this->get_new_arc_id (from, to);
    this->matrix[from][to].push_back (id);
    ++this->_arcs_count;
    return id;
}

void
ArrayDiGraph::remove_arc (ArcId id)
{
    /* We get the details of the arc: from and to */
    pair <NodeId, NodeId> &nodes = this->arcs[id];
    /* We get all the arcs between the given nodes */
    ArcIds &arcs = this->matrix[nodes.first][nodes.second];
    /* Go through those arcs and delete the good one */
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
ArrayDiGraph::list_successors (NodeId id)
{
    NodeIds successors;
    if (!this->node_exists[id])
        return successors;

    /* Get the line corresponding to given node */
    EndNodes &nodes = this->matrix[id];
    unsigned int index = 0;
    EndNodeIter start_iter = nodes.begin ();
    for (EndNodeIter to = start_iter, to_end = nodes.end (); to != to_end; to = start_iter + ++index)
    {
        /* filter out nodes which have been deleted */
        if (!this->node_exists[index])
            continue;
        /* If there are arcs going there, then it's a successor */
        if (!to->empty ())
            successors.push_back (index);
    }
    return successors;
}

NodeIds
ArrayDiGraph::list_ancestors (NodeId id)
{
    NodeIds ancestors;
    if (!this->node_exists[id])
        return ancestors;

    unsigned int index = 0;
    StartNodeIter start_iter = this->matrix.begin ();
    /* Go through all nodes */
    for (StartNodeIter from = start_iter, from_end = this->matrix.end (); from != from_end; from = start_iter + ++index)
    {
        /* filter out the deleted ones */
        if (!this->node_exists[index])
            continue;
        /* If there are arcs going to the node give, then we're an ancestor */
        if (!(*from)[id].empty ())
            ancestors.push_back (index);
    }
    return ancestors;
}

NodeIds
ArrayDiGraph::list_nodes ()
{
    NodeIds nodes;
    unsigned int index = 0;
    StartNodeIter start_iter = this->matrix.begin ();
    /* Go through all nodes, filtering deleted ones out */
    for (StartNodeIter from = start_iter, from_end = this->matrix.end (); from != from_end; from = start_iter + ++index)
    {
        if (this->node_exists[index])
            nodes.push_back (index);
    }
    return nodes;
}

ArcIds
ArrayDiGraph::list_arcs_from (NodeId id)
{
    /* This is similar to list_successors, but we add all the arcs instead of the node they're going to */
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
ArrayDiGraph::list_arcs_to (NodeId id)
{
    /* This is similar to list_ancestors, but we add all the arcs instead of the node they're leaving */
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
ArrayDiGraph::list_arcs_from_to (NodeId from, NodeId to)
{
    if (!this->node_exists[from] || !this->node_exists[to])
        return ArcIds ();
    return this->matrix[from][to];
}
