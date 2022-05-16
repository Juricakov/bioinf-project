#include "extensionScoreSorter.h"
#include "extensionScore.h"

ExtensionScoreSorter::ExtensionScoreSorter(std::unordered_map<string, Node *> lookup){
    this->lookup = lookup;
}

bool ExtensionScoreSorter::operator()(Edge* t1, Edge * t2) const {
    return ExtensionScoreHeuristic::compare(t1, t2, lookup);
}
