#include "heuristic.h"

class MonteCarloHeuristic : public Heuristic
{
private:
    /*

            012345 678 9
    QUERY = AAAAAA[AAA]G
            012 345
    TARGET = [AAA]BBC
    */
    static int getExtensionLength(Edge *edge, std::unordered_map<string, Node *> lookup);

    /*
    probabilities[i] is chosen if random number [0, 1] is in range <probabilities[i], probabilities[i + 1]].

                     0  1    2    3    4    5    6
    probabilities = [0, 0.2, 0.4, 0.5, 0.8, 0.9, 1];
    p = 0.3
    0.2 < 0.3 < 0.4 => select edge with index 1 (index of lower limit)
    */

    std::vector<float> probabilities;

public:
    MonteCarloHeuristic(std::vector<Edge *> edges, std::unordered_map<string, Node *> lookup);

    bool hasNext();
    Edge *getNext();
    Heuristic *createNextHeuristic(std::vector<Edge *> edges);
};