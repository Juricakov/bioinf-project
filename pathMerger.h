#include <string>
#include "path.h"
#include "vector"
#include <unordered_map>

class PathMerger
{
public:
    static Path *merge(std::vector<Path *> paths, std::unordered_map<std::string, Node *> lookup);
};