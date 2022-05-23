#include "overlapScore.h"
#include <iostream>

int OverlapScoreHeuristic::getOverlapLength(Edge *edge)
{
    return edge->queryEnd - edge->queryStart;
};

bool OverlapScoreHeuristic::compare(Edge *a, Edge *b)
{
    // have to return true if first argument is less than second
    // question from g: why, reverse works better on small example
    return getOverlapLength(a) > getOverlapLength(b);
}

OverlapScoreHeuristic::OverlapScoreHeuristic(std::vector<Edge *> edges) : Heuristic(edges)
{
    std::sort(this->edges.begin(), this->edges.end(), OverlapScoreHeuristic::compare);
    this->currentIndex = 0;
}

bool OverlapScoreHeuristic::hasNext() { return this->currentIndex < this->edges.size(); };

Edge *OverlapScoreHeuristic::getNext()
{
    if (!this->hasNext())
    {
        throw std::runtime_error("Overlap Score Heuristic has no more elements");
    }
    return this->edges[this->currentIndex++];
}

Heuristic *OverlapScoreHeuristic::createNextHeuristic(std::vector<Edge *> edges)
{
    return new OverlapScoreHeuristic(edges);
}