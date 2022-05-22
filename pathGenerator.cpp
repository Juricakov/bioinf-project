#include <iostream>
#include <unordered_map>
#include "pathGenerator.h"
#include "heuristic/heuristic.h"
#include "path.h"

// selection should be done after all paths have been generated
vector<Path *> PathGenerator::generate(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup)
{
    vector<Path *> allGeneratedPaths;

    for (int i = 0; i < 2 && heuristic->hasNext(); i++)
    {

        // cout << "NEW PATH" << endl;
        Path *path = new Path;

        // cout << "before get next" << endl;
        auto overlap = heuristic->getNext();
        // cout << "after get next" << endl;

        path->push(overlap);
        auto nextNode = lookup.at(overlap->getNeighbour(from->key));

        unordered_map<string, int> *visited = new unordered_map<string, int>;
        visited->insert({nextNode->id, 0});

        if (find(nextNode, heuristic->createNextHeuristic(nextNode->getOverlaps()), lookup, path, visited))
        {
            allGeneratedPaths.push_back(path);
        }
    }

    return allGeneratedPaths;
}

bool PathGenerator::find(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup, Path *path, unordered_map<string, int> *visited)
{

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

        if (visited->find(nextNode->id) != visited->end())
        {
            path->pop();
            continue;
        }

        visited->insert({nextNode->id, visited->at(from->id)});

        auto nextHeuristic = heuristic->createNextHeuristic(nextNode->getOverlaps());

        if (find(nextNode, nextHeuristic, lookup, path, visited))
        {
            return true;
        };

        path->pop();
    }
    return false;
}