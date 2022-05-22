#pragma once
#include "../Node.h"
#include "../Edge.h"
#include "vector"
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <iostream>

class Heuristic
{
protected:
    std::vector<Edge *> edges;

    Heuristic(std::vector<Edge *> edges)
    {
        this->edges = edges;
        // already filtered        
        // std::copy_if(edges.begin(), edges.end(), std::back_inserter(this->edges), [lookup, this](Edge *e)
        //              { return this->extends(e, lookup); });
    }

private:
    /*
            012345 678 9
    QUERY = AAAAAA[AAA]G
              012 345
    TARGET = [AAA]BBC
    */
    bool extends(Edge *edge, std::unordered_map<string, Node *> lookup)
    {
        auto queryNode = lookup[edge->querySequenceName];
        auto targetNode = lookup[edge->targetSequenceName];
        if (queryNode == nullptr)
        {
            throw std::invalid_argument("Missing query node in lookup");
        }
        if (targetNode == nullptr)
        {
            throw std::invalid_argument("Missing target node in lookup");
        }

        // ~ len BBC
        int baseCountAfterOverlapOnQuerySeq = queryNode->sequence.length() - edge->queryEnd;
        // ~ len G
        int baseCountAfterOverlapOnTargetSeq = targetNode->sequence.length() - edge->targetEnd;

        // will extend reading as long as len(BBC) > len(G)
        return baseCountAfterOverlapOnTargetSeq > baseCountAfterOverlapOnQuerySeq;
    }

public:
    virtual Edge *getNext() = 0;
    virtual bool hasNext() = 0;
    virtual Heuristic *createNextHeuristic(std::vector<Edge *> edges) = 0;
};