#include "pathMerger.h"
#include "Global.h"
#include <stdexcept>
#include <unordered_set>
#include <algorithm>
#include <iostream>

Path *PathMerger::merge(std::vector<Path *> paths, std::unordered_map<std::string, Node *> lookup)
{
    std::unordered_map<std::string, Path *> pathByStartNodeName;
    std::unordered_map<std::string, Path *> pathByEndNodeName;

    for (auto path : paths)
    {
        auto startNode = path->getStart(lookup);
        if (startNode->type != Type::CONTIG)
        {
            throw std::invalid_argument("Start node of path is not contig");
        }
        auto endNode = path->getEnd(lookup);
        if (endNode->type != Type::CONTIG)
        {
            throw std::invalid_argument("End node of path is not contig");
        }

        if (pathByStartNodeName[startNode->id] != nullptr)
        {
            throw std::invalid_argument("Multiple paths start from same contig");
        }
        pathByStartNodeName[startNode->id] = path;

        if (pathByEndNodeName[endNode->id] != nullptr)
        {
            throw std::invalid_argument("Multiple paths end on same contig");
        }
        pathByEndNodeName[endNode->id] = path;
    }

    std::vector<Path *> orderedPaths;
    std::unordered_set<Path *> unusedPaths(paths.begin(), paths.end());

    orderedPaths.push_back(paths[0]);
    unusedPaths.erase(paths[0]);

    // expand by finding prev path of first path in list, or next path of last path in list.
    while (true)
    {
        // looks like it never stops
        std::cout << "a" << std::endl;

        auto p = orderedPaths.front();
        auto prev = pathByEndNodeName[p->getStart(lookup)->id];
        if (prev != nullptr)
        {
            orderedPaths.insert(orderedPaths.begin(), prev);
            unusedPaths.erase(prev);
            continue;
        }
        p = orderedPaths.back();
        auto next = pathByStartNodeName[p->getEnd(lookup)->id];
        if (next != nullptr)
        {
            orderedPaths.push_back(next);
            unusedPaths.erase(next);
            continue;
        }

        // couldnt expand front or back
        if (unusedPaths.size() == 0)
        {
            break;
        }
        else
        {
            throw std::runtime_error("Could not expand path, but still have unmerged paths");
        }
    }

    Path *result = new Path();

    for (auto path : orderedPaths)
    {
        for (auto edge : path->getEdges())
        {
            result->push(edge);
        }
    }

    return result;
}