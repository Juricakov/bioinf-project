#include "extensionScore.h"
#include <iostream>

/*
    target sequence (S2) is extending query sequence (S1)
    we are getting extension score for S2

            012345 678 9
    QUERY = AAAAAA[AAA]G
            012 345
    TARGET = [AAA]BBC
*/

float getOverlapScore(Edge *edge)
{
    int OL1 = edge->queryEnd - edge->queryStart;
    int OL2 = edge->targetEnd - edge->targetStart;
    int SI = edge->numberOfMatchingBases;

    return (OL1 + OL2) * SI / 2;
};

float ExtensionScoreHeuristic::getExtensionScore(Edge *edge)
{
    int OS = getOverlapScore(edge);

    // ~ len BBC
    int OH1 = edge->querySeqeunceLength - edge->queryEnd;
    // ~ len G
    int OH2 = edge->targetSequenceLength - edge->targetEnd;

    // Extension length is len(BBC) - len(G);
    int EL2 = OH2 - OH1;

    return OS +
           (EL2 / 2) - ((OH1 + OH2) / 2);
};

bool ExtensionScoreHeuristic::compare(Edge *a, Edge *b)
{
    // comparator expects a < b to sort in ascending order, but we want ascending.
    return getExtensionScore(a) > getExtensionScore(b);
}

ExtensionScoreHeuristic::ExtensionScoreHeuristic(std::vector<Edge *> edges) : Heuristic(edges)
{
    std::sort(this->edges.begin(), this->edges.end(), ExtensionScoreHeuristic::compare);
    this->currentIndex = 0;
}

bool ExtensionScoreHeuristic::hasNext()
{
    return this->currentIndex < this->edges.size();
};

Edge *ExtensionScoreHeuristic::getNext()
{
    if (!this->hasNext())
    {
        throw std::runtime_error("Extension Score Heuristic has no more elements");
    }
    return this->edges[this->currentIndex++];
};

Heuristic *ExtensionScoreHeuristic::createNextHeuristic(std::vector<Edge *> edges)
{
    return new ExtensionScoreHeuristic(edges);
}