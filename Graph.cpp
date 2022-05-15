#include "Graph.h"
#include "fasta/namedSequence.h"
#include <memory>

bool Graph::addNode(Node *node)
{
    string nodeKey = (*node).key;

    if (nodeInGraph(nodeKey))
    {
        return false;
    }

    nodes[nodeKey] = node;

    if ((*node).type == Type::CONTIG)
    {
        contigs[nodeKey] = node;
    }

    return true;
}

bool Graph::nodeInGraph(string nodeKey)
{
    return nodes.find(nodeKey) != nodes.end();
}

bool Graph::contigInGraph(string nodeKey)
{
    return contigs.find(nodeKey) != contigs.end();
}

Node *Graph::getNode(string nodeKey)
{
    if (nodeInGraph(nodeKey))
    {
        return nodes.at(nodeKey);
    }
    return nullptr;
}

Node *Graph::getContig(string contigKey)
{
    if (contigInGraph(contigKey))
    {
        return contigs.at(contigKey);
    }
    return nullptr;
}

pair<Node *, Node *> Graph::getOrInitialize(string id, int length, Type type)
    {
        Node *regular = getNode(id);
        Node *complement = getNode(id + COMPLEMENT_SUFFIX);

        // both already exist
        if (regular != nullptr && complement != nullptr)
        {
            return make_pair(regular, complement);
        }

        // create both
        regular = new Node(
            type,
            id,
            length);

        complement = new Node(
            type,
            id,
            length,
            regular,
            true);

        regular->complement = complement;
        complement->complement = regular;

        addNode(regular);
        addNode(complement);

        return make_pair(regular, complement);
    }

void Graph::addSequences(vector<shared_ptr<NamedSequnce>> sequences){

    for (auto sequence: sequences){
        Node * node = this->getNode(sequence->name);
        
        if (node == nullptr){
            // is it possible that no edges were found for read/contig?
            // todo check and change exception
            throw runtime_error("should not be nullptr");
        }
        
        Node* complement = node->complement;
        
        node->sequence = sequence->sequence;
        complement->sequence = sequence->complement();
    }
}