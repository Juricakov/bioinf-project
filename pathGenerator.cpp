#include <iostream>
#include "pathGenerator.h"
#include "heuristic/heuristic.h"
#include "path.h"

// selection should be done after all paths have been generated
vector<Path *> PathGenerator::generate(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup)
{
    vector<Path *> allGeneratedPaths;

    for (int i = 0; i < 5 && heuristic->hasNext(); i++)
    {
        // cout << "NEW PATH" << endl;
        Path *path = new Path;

        // cout << "before get next" << endl;
        auto overlap = heuristic->getNext();
        // cout << "after get next" << endl;

        path->push(overlap);
        auto nextNode = lookup.at(overlap->getNeighbour(from->key));

        if (find(nextNode, heuristic->createNextHeuristic(nextNode->getOverlaps()), lookup, path))
        {
            allGeneratedPaths.push_back(path);
        }
    }

    return allGeneratedPaths;
}

bool PathGenerator::find(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup, Path *path)
{
    // cout << "start find" << endl;

    for (int i = 0; i < 5 && heuristic->hasNext(); i++)
    {
        Edge *bestOverlap = heuristic->getNext();
        path->push(bestOverlap);

        auto nextNode = lookup.at(bestOverlap->getNeighbour(from->key));

        // if (path->size() % 10 == 0)
        // {
        //     cout << "still alive " << path->size() << endl;
        //     cout << nextNode->key << endl;
        // }

        if (nextNode->type == Type::CONTIG)
        {
            cout << "FOUND " << path->size() << " " << nextNode->key << endl;
            return true;
        }

        auto nextHeuristic = heuristic->createNextHeuristic(nextNode->getOverlaps());

        if (find(nextNode, nextHeuristic, lookup, path))
        {
            return true;
        };

        path->pop();
    }
    return false;
}