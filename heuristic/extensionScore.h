#include "heuristic.h"

class ExtensionScoreHeuristic : public Heuristic
{
private:
    static int getExtensionLength(Edge *edge);
    int currentIndex = 0;

public:
    ExtensionScoreHeuristic(std::vector<Edge *> edges);

    static bool compare(Edge *a, Edge *b);
    bool hasNext();
    Edge *getNext();
    Heuristic *createNextHeuristic(std::vector<Edge *> edges);
};