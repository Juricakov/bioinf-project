#include "heuristic.h"

class OverlapScoreHeuristic : public Heuristic
{
private:
    int currentIndex;
    static int getOverlapLength(Edge *edge);

    static bool compare(Edge *a, Edge *b);
    
public:
    OverlapScoreHeuristic(std::vector<Edge *> edges);

    bool hasNext();
    Edge *getNext();
    Heuristic *createNextHeuristic(std::vector<Edge *> edges);
};