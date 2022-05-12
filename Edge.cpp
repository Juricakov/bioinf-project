#include "Edge.h"

Edge::Edge(string query, string target, int qStart, int qEnd, char strand,
           int tStart, int tEnd, int numMatches, int allignLen, int quality)
{
    querySequenceName = query;
    targetSequenceName = target;
    queryStart = qStart;
    queryEnd = qEnd;
    relativeStrand = strand;
    targetStart = tStart;
    targetEnd = tEnd;
    numberOfMatches = numMatches;
    alignmentBlockLengt = allignLen;
    mappingQuality = quality;
}

string Edge::getNeighbour(string sequenceName)
{
    return (this->querySequenceName == sequenceName) ? targetSequenceName : querySequenceName;
}
