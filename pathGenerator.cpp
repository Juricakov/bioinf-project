#include <iostream>
#include "pathGenerator.h"
#include "heuristic/heuristic.h"
#include "path.h"

// selection should be done after all paths have been generated
vector<Path *> PathGenerator::generate(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup)
{
    vector<Path *> allGeneratedPaths;

    while (heuristic->hasNext())
    {
        Path *path = new Path;
        auto overlap = heuristic->getNext();
        path->push(overlap);
        auto nextNode = lookup.at(overlap->getNeighbour(from->key));
        if (find(nextNode, heuristic->createNextHeuristic(nextNode->overlaps), lookup, path))
        {
            allGeneratedPaths.push_back(path);
        }
    }

    return allGeneratedPaths;
}

// fake to string for edge, for testing purposes
ostream &operator<<(std::ostream &strm, const Edge &e)
{
    return strm << "Edge(" << e.querySequenceName << ", " << e.queryStart << ", "
                << e.queryEnd << ", " << e.relativeStrand << ", " << e.targetSequenceName
                << ", " << e.targetStart << ", " << e.targetEnd << ", "
                << e.alignmentBlockLength << ")";
}

bool PathGenerator::find(Node *from, Heuristic *heuristic, unordered_map<string, Node *> lookup, Path *path)
{
    // checking all possible overlaps, maybe too slow/too much work, determine after testing
    // possible to set hasNext() to false after some number of fetches (eg. after 5 getNext return no next)

    // if (path->size() % 10 == 0)
    // {
    //     cout << path->size() << endl;
    // }

    while (heuristic->hasNext())
    {
        Edge *bestOverlap = heuristic->getNext();
        path->push(bestOverlap);

        auto nextNode = lookup.at(bestOverlap->getNeighbour(from->key));

        if (nextNode->type == Type::CONTIG)
        {
            cout << "FOUND " << path->size() << " " << nextNode->key << endl;
            return true;
        }

        auto nextHeuristic = heuristic->createNextHeuristic(nextNode->overlaps);

        if (find(nextNode, nextHeuristic, lookup, path))
        {
            return true;
        };

        if (path->isDeadEnd(lookup))
        {
            path->pop();
        }
    }
    return false;
}