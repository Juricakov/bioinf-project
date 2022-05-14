#include "vector"
#include "Node.h"
#include "Edge.h"
#include <unordered_map>

class Path
{
private:
    std::vector<Edge *> path;

public:
    Path();
    bool isDeadEnd(std::unordered_map<string, Node *> lookup);
    Edge *pop();
    void push(Edge *edge);
    Node *getStart(std::unordered_map<string, Node *> lookup);
    Node *getEnd(std::unordered_map<string, Node *> lookup);
    int size();
    std::vector<Edge *> getEdges();
};
