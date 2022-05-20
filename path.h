#include "vector"
#include "Node.h"
#include "Edge.h"
#include <unordered_map>

class Path
{
private:
    std::vector<Edge *> path;
    int id;
    static int current_id;

public:
    Path()
    {
        this->id = current_id++;
    }

    bool isDeadEnd(std::unordered_map<string, Node *> lookup);
    Edge *pop();
    int getId()
    {
        return this->id;
    }
    void push(Edge *edge);
    Node *getStart(std::unordered_map<string, Node *> lookup);
    std::string getStartNodeName();
    Node *getEnd(std::unordered_map<string, Node *> lookup);
    std::string getEndNodeName();
    int size();
    std::vector<Edge *> getEdges();
};
