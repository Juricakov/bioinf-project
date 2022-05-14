#include <string>

using namespace std;

// edges represent overlaps
class Edge
{
public:
    std::string querySequenceName;
    std::string targetSequenceName;
    // where overlap starts on query
    int queryStart;
    int queryEnd;

    // regular or complement
    char relativeStrand;

    // takes complementing in the account?
    int targetStart;
    int targetEnd;

    int alignmentBlockLengt;

    Edge();

    Edge(string query, string target, int qStart, int qEnd, char strand,
         int tStart, int tEnd, int numMatches, int allignLen, int quality);

    string getNeighbour(string sequenceName);
};