#include "path.h"
#include "vector"
#include "Node.h"
#include "heuristic.h"
#include "pathSelector.h"

// TODO Grigor implement
class PathGenerator
{
public:
    static Path *generate(Node *from, Heuristic heuristic, PathSelector selector);
};