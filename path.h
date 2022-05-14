#include "Edge.h"
#include "vector"
#include "Node.h"
// TODO Jurica implement
class Path
{
private:
    std::vector<Edge *> path;

public:
    Path();
    bool isDeadEnd();
    Edge *pop();
    void push(Edge *edge);
    Node *getStart();
    Node *getEnd();
    int size();
};

// Path klasa definicija
// Heuristic definicija (sve 3)
// Dodat u Edge pointere na nodeove.
// dodat sequence u Node
