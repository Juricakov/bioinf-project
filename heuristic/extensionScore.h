#include "heuristic.h"

class ExtensionScoreHeuristic : public Heuristic
{
private:
    static int getExtensionLength(Edge *edge, Node *queryNode, Node *targetNode);
    int currentIndex = 0;

public:
    ExtensionScoreHeuristic(std::vector<Edge *> edges, std::unordered_map<string, Node *> lookup);

    static bool compare(Edge *a, Edge *b, std::unordered_map<string, Node *> lookup);
    bool hasNext();
    Edge *getNext();
    Heuristic *createNextHeuristic(std::vector<Edge *> edges);
};