#include <numeric>
#include <stdlib.h>
#include "monteCarlo.h"

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

float MonteCarloHeuristic::getExtensionScore(Edge *edge)
{
    int OS = getOverlapScore(edge);

    // ~ len BBC
    int OH1 = edge->querySeqeunceLength - edge->queryEnd;
    // ~ len G
    int OH2 = edge->targetSequenceLength - edge->targetEnd;

    // Extension length is len(BBC) - len(G);
    int EL2 = OH2 - OH1;

    return OS +
           EL2 / 2 - (OH1 + OH2) / 2;
};

MonteCarloHeuristic::MonteCarloHeuristic(std::vector<Edge *> edges) : Heuristic(edges)
{
    std::vector<float> extensionScores;
    std::transform(this->edges.begin(), this->edges.end(), std::back_inserter(extensionScores), [](Edge *e) -> int
                   { return getExtensionScore(e); });

    float extensionSum = std::accumulate(extensionScores.begin(), extensionScores.end(), 0);
    this->probabilities.push_back(0);
    for (int i = 0; i < this->edges.size(); i++)
    {
        float range = extensionScores[i] / extensionSum; // probability will be proportional to extension length
        this->probabilities.push_back(this->probabilities.at(i) + range);
    }
}

bool MonteCarloHeuristic::hasNext()
{
    return !this->edges.empty();
}

Edge *MonteCarloHeuristic::getNext()
{
    if (!this->hasNext())
    {
        throw std::runtime_error("Monte Carlo Heuristic has no more elements");
    }

    float p = ((float)std::rand() / (RAND_MAX));
    for (int i = 0; i < probabilities.size() - 1; i++)
    {
        if (this->probabilities[i] < p && p <= this->probabilities[i + 1])
        {
            return this->edges[i];
        }
    }
    // rounding error caused code to get to this point, should have returned final element.
    return this->edges.at(this->edges.size() - 1);
}

Heuristic *MonteCarloHeuristic::createNextHeuristic(std::vector<Edge *> edges)
{
    return new MonteCarloHeuristic(edges);
}
