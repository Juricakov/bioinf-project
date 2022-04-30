#include <string>
#include <vector>
#include "Global.h"
#include "Edge.h"

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

    // used for storing complements in dictionary
    // id is same but when merging complement needs to be calculated
    // for regular nodes key and id are identical
    string key;

    int length;
    vector<Edge *> overlaps;
    Node *complement = nullptr;

    // needed for search
    float quality = 0;
    Node *previous = nullptr;

    Node(){};

    // key is automatically set to same value as id
    Node(Type t, string name, int len);

    // constructor for complements, automatic key change
    Node(Type t, string name, int len, Node *c, bool isCompl);

    void addOverlap(Edge *overlap);

    bool isComplement();
};