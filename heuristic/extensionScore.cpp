#include "extensionScore.h"
#include <iostream>

int ExtensionScoreHeuristic::getExtensionLength(Edge *edge)
{
    // ~ len BBC
    int baseCountAfterOverlapOnQuerySeq = edge->querySeqeunceLength - edge->queryEnd;
    // ~ len G
    int baseCountAfterOverlapOnTargetSeq = edge->targetSequenceLength - edge->targetEnd;

    // Extension length is len(BBC) - len(G);
    return baseCountAfterOverlapOnQuerySeq - baseCountAfterOverlapOnTargetSeq;
};

bool ExtensionScoreHeuristic::compare(Edge *a, Edge *b)
{
    // have to return true if first argument is less than second ??
    return getExtensionLength(a) < getExtensionLength(b);
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