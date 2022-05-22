#pragma once
#include <string>
#include <vector>
#include "Global.h"
#include "Edge.h"
#include <unordered_map>

using namespace std;

// nodes represent reads
// in hera paper complementary nodes are also connected, should not be possible to use
// both original and complement
// id is id of the read/contig, will be used for merging
class Node
{
public:
    Type type;
    string id;
    string sequence;

    // used for storing complements in dictionary
    // id is same but when merging complement needs to be calculated
    // for regular nodes key and id are identical
    string key;

    unordered_map<string, Edge *> overlaps;
    Node *complement = nullptr;

    Node(){};

    // key is automatically set to same value as id
    Node(Type t, string name);

    // constructor for complements, automatic key change
    Node(Type t, string name, Node *c, bool isCompl);

    void addOverlap(Edge *overlap);

    bool isComplement();

    vector<Edge *> getOverlaps();
};