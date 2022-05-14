#include "sequenceGenerator.h"
#include <numeric>

std::string SequenceGenerator::generate(Path *path, std::unordered_map<std::string, Node *> lookup)
{
    auto startNode = path->getStart(lookup);
    std::vector<std::string> sequencePieces;

    sequencePieces.push_back(startNode->sequence);

    for (auto edge : path->getEdges())
    {
        auto nextNode = lookup[edge->targetSequenceName];
        if (edge->relativeStrand == '-')
        {
            nextNode = nextNode->complement;
        }

        sequencePieces.push_back(nextNode->sequence.substr(edge->targetStart, edge->targetEnd - edge->targetStart));
    }

    return std::accumulate(sequencePieces.begin(), sequencePieces.end(), std::string(""));
}