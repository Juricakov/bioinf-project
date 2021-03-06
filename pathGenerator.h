#include "path.h"
#include "vector"
#include "Node.h"
#include "heuristic/heuristic.h"
#include "pathSelector.h"
#include <unordered_set>

class PathGenerator
{
public:
    static vector<Path *> generate(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup, int maxNumPaths);
    static bool find(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup, Path *path, unordered_set<string> *visited);
};