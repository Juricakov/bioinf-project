#include "heuristic.h"
#include <numeric>
#include <stdlib.h>

class MonteCarloHeuristic : Heuristic
{
private:
    /*

            012345 678 9
    QUERY = AAAAAA[AAA]G
            012 345
    TARGET = [AAA]BBC
    */
    static int getExtensionLength(Edge *edge, std::unordered_map<string, Node *> lookup)
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

    /*

    probabilities[i] is chosen if random number [0, 1] is in range <probabilities[i], probabilities[i + 1]].

                     0  1    2    3    4    5    6
    probabilities = [0, 0.2, 0.4, 0.5, 0.8, 0.9, 1];
    p = 0.3
    0.2 < 0.3 < 0.4 => select edge with index 1 (index of lower limit)
    */

    std::vector<float> probabilities;

public:
    MonteCarloHeuristic(std::vector<Edge *> edges, std::unordered_map<string, Node *> lookup) : Heuristic(edges, lookup)
    {
        std::vector<int> extensionLengths;
        std::transform(edges.begin(), edges.end(), std::back_inserter(extensionLengths), [lookup](Edge *e) -> int
                       { return getExtensionLength(e, lookup); });

        int extensionSum = std::accumulate(extensionLengths.begin(), extensionLengths.end(), 0);

        this->probabilities.push_back(0);
        for (int i = 0; i < edges.size(); i++)
        {
            int range = extensionLengths[i] / extensionSum; // probability will be proportional to extension length

            this->probabilities.push_back(this->probabilities.at(i) + range);
        }
    }

    bool hasNext()
    {
        return true;
    }

    Edge *getNext()
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
};