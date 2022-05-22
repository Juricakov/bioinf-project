#include "Graph.h"
#include "namedSequence.h"
#include <memory>
#include <regex>


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

pair<Node *, Node *> Graph::getOrInitialize(string id, Type type)
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
        id);

    complement = new Node(
        type,
        id,
        regular,
        true);

    regular->complement = complement;
    complement->complement = regular;

    addNode(regular);
    addNode(complement);

    return make_pair(regular, complement);
}

vector<string> splitGraph(const string str, const string regex_str)
{
    regex reg(regex_str);
    vector<string> list(sregex_token_iterator(str.begin(), str.end(), reg, -1),
                        sregex_token_iterator());
    return list;
}

void Graph::addSequences(vector<shared_ptr<NamedSequnce>> sequences)
{

    for (auto sequence : sequences)
    {


        Node *node = this->getNode(splitGraph(sequence->name, " ").at(0));

        if (node == nullptr)
        {
            // is it possible that no edges were found for read/contig?
            // happens in e coli test data
            continue;
        }

        Node *complement = node->complement;

        node->sequence = sequence->sequence;
        complement->sequence = sequence->complement();
    }
}