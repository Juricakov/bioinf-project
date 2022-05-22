#include "sequenceGenerator.h"
#include <numeric>
#include <iostream>

std::string SequenceGenerator::generate(Path *path, std::unordered_map<std::string, Node *> lookup)
{
    auto startNode = path->getStart(lookup);
    std::vector<std::string> sequencePieces;

    sequencePieces.push_back(startNode->sequence);

    for (auto edge : path->getEdges())
    {
        auto nextNode = lookup[edge->targetSequenceName];
        auto numSteps = edge->targetEnd - edge->targetStart;
        numSteps = nextNode->sequence.length() - edge->targetEnd;
        auto substr = nextNode->sequence.substr(edge->targetEnd, numSteps);
        sequencePieces.push_back(substr);
    }

    return std::accumulate(sequencePieces.begin(), sequencePieces.end(), std::string(""));
}