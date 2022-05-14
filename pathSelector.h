#include "path.h"
#include "vector"
#include "Node.h"

// TODO Jurica implement
class PathSelector
{
public:
    virtual Path *pick(std::vector<Path *> paths) = 0;
};