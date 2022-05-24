#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include "pathGenerator.h"
#include "heuristic/heuristic.h"
#include "path.h"

// selection should be done after all paths have been generated
vector<Path *> PathGenerator::generate(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup, int maxNumPaths)
{
    vector<Path *> allGeneratedPaths;

    for (int i = 0; i < maxNumPaths && heuristic->hasNext(); i++)
    {

        // cout << "NEW PATH" << endl;
        Path *path = new Path;

        // cout << "before get next" << endl;
        auto overlap = heuristic->getNext();
        // cout << "after get next" << endl;

        path->push(overlap);
        auto nextNode = lookup.at(overlap->getNeighbour(from->key));

        unordered_set<string> *visited = new unordered_set<string>;
        visited->insert(from->id);
        visited->insert(nextNode->id);

        if (find(nextNode, heuristic->createNextHeuristic(nextNode->getOverlaps()), lookup, path, visited))
        {
            allGeneratedPaths.push_back(path);
        }
        else {
            cout << "NO PATH FOUND" << endl;
        }
    }

    return allGeneratedPaths;
}

bool PathGenerator::find(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup, Path *path, unordered_set<string> *visited)
{

    for (int i = 0; i < 5 && heuristic->hasNext(); i++)
    {
        Edge *bestOverlap = heuristic->getNext();
        path->push(bestOverlap);

        auto nextNode = lookup.at(bestOverlap->getNeighbour(from->key));

        if (visited->find(nextNode->id) != visited->end())
        {
            path->pop();
            continue;
        }

        if (nextNode->type == Type::CONTIG)
        {
            cout << "FOUND " << path->size() << " " << nextNode->key << endl;
            return true;
        }

        visited->insert(nextNode->id);

        auto nextHeuristic = heuristic->createNextHeuristic(nextNode->getOverlaps());

        if (find(nextNode, nextHeuristic, lookup, path, visited))
        {
            return true;
        };

        path->pop();
    }
    return false;
}