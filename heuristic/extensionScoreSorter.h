#include <unordered_map>
#include <string>
#include "../Node.h"
#include "../Edge.h"

class ExtensionScoreSorter {
    std::unordered_map<string, Node *> lookup;

public:
    ExtensionScoreSorter(std::unordered_map<string, Node *> lookup);
    bool operator()(Edge* t1, Edge * t2) const;  
};