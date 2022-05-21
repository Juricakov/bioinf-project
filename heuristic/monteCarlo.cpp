#include <numeric>
#include <stdlib.h>
#include "monteCarlo.h"
#include "extensionScoreSorter.h"

int MonteCarloHeuristic::getExtensionLength(Edge *edge, std::unordered_map<string, Node *> lookup)
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

    // ~ len BBC
    int baseCountAfterOverlapOnQuerySeq = queryNode->sequence.length() - edge->queryEnd - 1;
    // ~ len G
    int baseCountAfterOverlapOnTargetSeq = targetNode->sequence.length() - edge->targetEnd - 1;

    // Extension length is len(BBC) - len(G);
    return baseCountAfterOverlapOnQuerySeq - baseCountAfterOverlapOnTargetSeq;
};

MonteCarloHeuristic::MonteCarloHeuristic(std::vector<Edge *> edges, std::unordered_map<string, Node *> lookup) : Heuristic(edges, lookup)
{
    std::vector<int> extensionLengths;
    std::transform(this->edges.begin(), this->edges.end(), std::back_inserter(extensionLengths), [lookup](Edge *e) -> int
                   { return getExtensionLength(e, lookup); });

    int extensionSum = std::accumulate(extensionLengths.begin(), extensionLengths.end(), 0);
    this->probabilities.push_back(0);
    for (int i = 0; i < this->edges.size(); i++)
    {
        float range = (float)extensionLengths[i] / extensionSum; // probability will be proportional to extension length
        this->probabilities.push_back(this->probabilities.at(i) + range);
    }
}

bool MonteCarloHeuristic::hasNext()
{
    return this->edges.empty() ? false : true;
}

Edge *MonteCarloHeuristic::getNext()
{
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
    return new MonteCarloHeuristic(edges, this->lookup);
}
