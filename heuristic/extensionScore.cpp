#include "heuristic.h"

class ExtensionScoreHeuristic : Heuristic
{
private:
    /*

            012345 678 9
    QUERY = AAAAAA[AAA]G
              012 345
    TARGET = [AAA]BBC
    */
    static int getExtensionLength(Edge *edge, Node *queryNode, Node *targetNode)
    {
        // ~ len BBC
        int baseCountAfterOverlapOnQuerySeq = queryNode->sequence.length() - edge->queryEnd;
        // ~ len G
        int baseCountAfterOverlapOnTargetSeq = targetNode->sequence.length() - edge->targetEnd;

        // Extension length is len(BBC) - len(G);
        return baseCountAfterOverlapOnQuerySeq - baseCountAfterOverlapOnTargetSeq;
    };

    static bool compare(Edge *a, Edge *b, std::unordered_map<string, Node *> lookup)
    {
        auto queryNodeA = lookup[a->querySequenceName];
        auto targetNodeA = lookup[a->targetSequenceName];
        auto queryNodeB = lookup[b->querySequenceName];
        auto targetNodeB = lookup[b->targetSequenceName];

        if (queryNodeA == nullptr)
        {
            throw std::invalid_argument("Missing query node A in lookup");
        }
        if (targetNodeA == nullptr)
        {
            throw std::invalid_argument("Missing target node A in lookup");
        }

        if (queryNodeB == nullptr)
        {
            throw std::invalid_argument("Missing query node B in lookup");
        }
        if (targetNodeB == nullptr)
        {
            throw std::invalid_argument("Missing target node B in lookup");
        }
        // have to return true if first argument is less than  second
        return getExtensionLength(a, queryNodeA, targetNodeA) < getExtensionLength(b, queryNodeB, targetNodeB);
    }
    int currentIndex = 0;

public:
    ExtensionScoreHeuristic(std::vector<Edge *> edges, std::unordered_map<string, Node *> lookup) : Heuristic(edges, lookup)
    {
        std::sort(this->edges.begin(), this->edges.end(), ExtensionScoreHeuristic::compare);
        this->currentIndex = 0;
    }

    bool hasNext() { return this->currentIndex < this->edges.size(); };
    Edge *getNext()
    {
        if (!this->hasNext())
        {
            throw std::runtime_error("Extension Score Heuristic has no more elements");
        }
        return this->edges[this->currentIndex++];
    };
};