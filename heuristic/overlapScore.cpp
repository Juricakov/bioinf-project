#include "heuristic.h"

class OverlapScoreHeuristic : Heuristic
{
private:
    int currentIndex;
    static int getOverlapLength(Edge *edge)
    {
        return edge->queryEnd - edge->queryStart;
    };

    static bool compare(Edge *a, Edge *b)
    {
        // have to return true if first argument is less than  second
        return getOverlapLength(a) < getOverlapLength(b);
    }

public:
    OverlapScoreHeuristic(std::vector<Edge *> edges, std::unordered_map<string, Node *> lookup) : Heuristic(edges, lookup)
    {
        std::sort(this->edges.begin(), this->edges.end(), OverlapScoreHeuristic::compare);
        this->currentIndex = 0;
    }

    bool hasNext() { return this->currentIndex < this->edges.size(); };
    Edge *getNext() { return this->edges[this->currentIndex++]; };
};