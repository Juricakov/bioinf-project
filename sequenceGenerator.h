#include "path.h"
#include <vector>
#include <string>
#include <unordered_map>

class SequenceGenerator
{
public:
    static std::string generate(Path *p, std::unordered_map<std::string, Node *> lookup);
};