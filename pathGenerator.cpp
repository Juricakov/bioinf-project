#include <iostream>
#include "pathGenerator.h"
#include "heuristic/heuristic.h"
#include "path.h"

// selection should be done after all paths have been generated
vector<Path *> PathGenerator::generate(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup)
{
    vector<Path *> allGeneratedPaths;

    for (int i = 0; i < 2 && heuristic->hasNext(); i++)
    {
        cout << "NEW PATH" << endl;
        Path *path = new Path;
        auto overlap = heuristic->getNext();
        path->push(overlap);
        auto nextNode = lookup.at(overlap->getNeighbour(from->key));

        vector<Edge *> v;

        for (auto it = nextNode->overlaps.begin(); it != nextNode->overlaps.end(); ++it)
        {
            v.push_back(it->second);
        }

        if (find(nextNode, heuristic->createNextHeuristic(v), lookup, path))
        {
            allGeneratedPaths.push_back(path);
        }
        else
        {
            cout << "NO PATH FOUND " << endl;
        }
    }

    return allGeneratedPaths;
}

bool PathGenerator::find(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup, Path *path)
{
    for (int i = 0; i < 50 && heuristic->hasNext(); i++)
    {
        Edge *bestOverlap = heuristic->getNext();
        path->push(bestOverlap);

        auto nextNode = lookup.at(bestOverlap->getNeighbour(from->key));

        if (nextNode->type == Type::CONTIG)
        {
            cout << "FOUND " << path->size() << " " << nextNode->key << endl;
            return true;
        }

        vector<Edge *> v;

        for (auto it = nextNode->overlaps.begin(); it != nextNode->overlaps.end(); ++it)
        {
            v.push_back(it->second);
        }

        auto nextHeuristic = heuristic->createNextHeuristic(v);

        if (find(nextNode, nextHeuristic, lookup, path))
        {
            return true;
        };

        path->pop();
    }
    return false;
}