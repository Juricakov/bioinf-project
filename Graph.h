#include <unordered_map>
#include <iostream>
#include <memory>
#include "Node.h"
#include "namedSequence.h"

using namespace std;

class Graph
{
public:
    // all nodes, map for fast searching by key
    unordered_map<string, Node *> nodes;

    // just contigs, could be useful in algorithm implementation
    // remove if memory usage too big (there should be only few contigs
    // so this will probably not cause problems)
    unordered_map<string, Node *> contigs;

    // adds node if it is not already in the graph
    // returns true if added
    bool addNode(Node *node);

    bool nodeInGraph(string nodeKey);

    bool contigInGraph(string nodeKey);

    Node *getNode(string nodeKey);

    Node *getContig(string contigKey);

    // if present return otherwise create new one and return
    // always create pairs, first is regular, second complement
    pair<Node *, Node *> getOrInitialize(string id, Type type);

    void addSequences(vector<shared_ptr<NamedSequnce>>);
};
