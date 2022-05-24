#include "overlapScore.h"
#include <iostream>

/*
    target sequence (S2) is extending query sequence (S1)
    we are getting extension score for S2

            012345 678 9
    QUERY = AAAAAA[AAA]G
            012 345
    TARGET = [AAA]BBC
*/

float OverlapScoreHeuristic::getOverlapScore(Edge *edge)
{
    int OL1 = edge->queryEnd - edge->queryStart;
    int OL2 = edge->targetEnd - edge->targetStart;
    int SI = edge->numberOfMatchingBases/edge->alignmentBlockLength;

    return (OL1 + OL2) * SI / 2;
};

bool OverlapScoreHeuristic::compare(Edge *a, Edge *b)
{
    // comparator expects a < b to sort in ascending order, but we want descending.
    return getOverlapScore(a) > getOverlapScore(b);
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