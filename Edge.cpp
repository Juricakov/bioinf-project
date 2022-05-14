#include "Edge.h"

// TODO Grigor update / implement
Edge::Edge(string query, string target, int qStart, int qEnd, char strand,
           int tStart, int tEnd, int allignLen)
{
    querySequenceName = query;
    targetSequenceName = target;
    queryStart = qStart;
    queryEnd = qEnd;
    relativeStrand = strand;
    targetStart = tStart;
    targetEnd = tEnd;
    alignmentBlockLength = allignLen;
}

string Edge::getNeighbour(string sequenceName)
{
    return (this->querySequenceName == sequenceName) ? targetSequenceName : querySequenceName;
}
