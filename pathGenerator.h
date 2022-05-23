#include "path.h"
#include "vector"
#include "Node.h"
#include "heuristic/heuristic.h"
#include "pathSelector.h"

class PathGenerator
{
public:
    static vector<Path *> generate(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup);
    static bool find(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup, Path *path, unordered_map<string, int> *visited);
};