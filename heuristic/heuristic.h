#include "Node.h"
#include "Edge.h"
#include "vector"
// TODO Jurica implement
class Heuristic
{
private:
    bool extends(Node *node, Edge *edge)
    {
        // querySequence.length - querySequence.startIndex > targetSeq.length - targetSeq.startIndex
        return false;
    }

public:
    virtual std::vector<Edge *> sort(Node *start, std::vector<Edge *> overlaps) = 0;
};