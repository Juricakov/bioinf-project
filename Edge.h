#pragma once
#include <string>
#include <iostream>

using namespace std;

// edges represent overlaps
class Edge
{
public:
    string querySequenceName;
    string targetSequenceName;
    // where overlap starts on query
    int queryStart;
    int queryEnd;

    // regular or complement
    char relativeStrand;

    // takes complementing in the account?
    int targetStart;
    int targetEnd;

    int alignmentBlockLength;

    Edge();

    Edge(string query, string target, int qStart, int qEnd, char strand,
         int tStart, int tEnd, int allignLen);

    string getNeighbour(string sequenceName);

    Edge *flipQueryAndTarget();
};