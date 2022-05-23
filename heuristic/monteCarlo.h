#include "heuristic.h"

class MonteCarloHeuristic : public Heuristic
{
private:
    static float getExtensionScore(Edge *edge);

    /*
    probabilities[i] is chosen if random number [0, 1] is in range <probabilities[i], probabilities[i + 1]].

                     0  1    2    3    4    5    6
    probabilities = [0, 0.2, 0.4, 0.5, 0.8, 0.9, 1];
    p = 0.3
    0.2 < 0.3 < 0.4 => select edge with index 1 (index of lower limit)
    */

    std::vector<float> probabilities;

public:
    MonteCarloHeuristic(std::vector<Edge *> edges);

    bool hasNext();
    Edge *getNext();
    Heuristic *createNextHeuristic(std::vector<Edge *> edges);
};