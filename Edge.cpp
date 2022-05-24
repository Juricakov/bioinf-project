#include "Edge.h"

Edge::Edge(){};

Edge::Edge(string query, string target, int qStart, int qEnd, char strand,
           int tStart, int tEnd, int allignLen, int queryLen, int targetLen, int numMatches)
{
    querySequenceName = query;
    targetSequenceName = target;
    queryStart = qStart;
    queryEnd = qEnd;
    relativeStrand = strand;
    targetStart = tStart;
    targetEnd = tEnd;
    alignmentBlockLength = allignLen;
    querySeqeunceLength = queryLen;
    targetSequenceLength = targetLen;
    numberOfMatchingBases = numMatches;
}

string Edge::getNeighbour(string sequenceName)
{
    return (this->querySequenceName == sequenceName) ? targetSequenceName : querySequenceName;
}

Edge * Edge::flipQueryAndTarget(){
    Edge * newEdge = new Edge;

    newEdge->alignmentBlockLength = alignmentBlockLength;
    newEdge->relativeStrand = relativeStrand;
    
    newEdge->queryEnd = targetEnd;
    newEdge->queryStart = targetStart;
    newEdge->querySequenceName = targetSequenceName;
    
    newEdge->targetEnd = queryEnd;
    newEdge->targetStart = queryStart;
    newEdge->targetSequenceName = querySequenceName;

    newEdge->querySeqeunceLength = targetSequenceLength;
    newEdge->targetSequenceLength = querySeqeunceLength;

    newEdge->numberOfMatchingBases = numberOfMatchingBases;

    return newEdge;
}
