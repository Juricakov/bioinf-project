#include "Node.h"
#include "Edge.h"
#include "vector"
#include <unordered_map>
#include <stdexcept>

// TODO Jurica implement
class Heuristic
{
private:
    bool extends(Edge *edge, std::unordered_map<string, Node *> lookup)
    {
        auto queryNode = lookup[edge->querySequenceName];
        auto targetNode = lookup[edge->targetSequenceName];
        if (queryNode == nullptr)
        {
            throw std::invalid_argument("Missing query node in lookup");
        }
        if (targetNode == nullptr)
        {
            throw std::invalid_argument("Missing target node in lookup");
        }

        return queryNode->sequence.length() - edge->queryStart > targetNode->sequence.length() - edge->targetStart;
    }

public:
    virtual std::vector<Edge *> sortOverlaps(Node *node, std::unordered_map<string, Node *> lookup) = 0;
};