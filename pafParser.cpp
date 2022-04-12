#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <regex>

#define CONTIG_SUFFIX "Cmpl"

using namespace std;

// read PAF file
// create node objects
// create overlap objects
// add overlap data pointers to relevant nodes
/*

node 1 = {
    contig1
    overlaps: [overlap1ptr, overlap 2ptr]
}

node 2 = {
    reading 1
    overlaps [overlap1ptr]
}

node 3 = {
    reading 2
    overlaops [overlap 2ptr]
}
*/

// code sketch, classes should be in seperate files

enum Type
{
    CONTIG,
    READ
};

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

    int numberOfMatches;
    int alignmentBlockLengt;
    int mappingQuality;

    Edge(){};

    Edge(string query, string target, int qStart, int qEnd, char strand,
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

    string getNeighbour(string sequenceName)
    {
        return (querySequenceName == sequenceName) ? targetSequenceName : querySequenceName;
    }
};

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

    Node(){};

    // key is automatically set to same value as id
    Node(Type t, string name, int len)
    {
        type = t;
        id = name;
        key = name;
        length = len;
    };

    // constructor for complements, automatic key change
    Node(Type t, string name, int len, Node *c, bool isCompl)
    {
        string newKey = name;
        if (isCompl)
        {
            newKey = newKey + CONTIG_SUFFIX;
        }

        type = t;
        id = name;
        key = newKey;
        complement = c;
        length = len;
    };

    void addOverlap(Edge *overlap)
    {
        overlaps.push_back(overlap);
    }

    bool isComplement()
    {
        return !(key == id);
    }
};

class Graph
{
public:
    // all nodes, map for fast searching by key
    unordered_map<string, Node *> nodes;

    // just contigs, could be useful in algorithm implementation
    // remove if memory usage too big (there should be only few contigs
    // so this will probably not cause problems)
    unordered_map<string, Node *> contigs;

    // adds node if it is not already in the graph
    // returns true if added
    bool addNode(Node *node)
    {
        string nodeKey = (*node).key;

        if (nodeInGraph(nodeKey))
        {
            return false;
        }

        nodes[nodeKey] = node;

        if ((*node).type == CONTIG)
        {
            contigs[nodeKey] = node;
        }

        return true;
    }

    bool nodeInGraph(string nodeKey)
    {
        return nodes.find(nodeKey) != nodes.end();
    }

    Node *getNode(string nodeKey)
    {
        if (nodeInGraph(nodeKey))
        {
            return nodes.at(nodeKey);
        }
        return nullptr;
    }

    Node *getContig(string contigKey)
    {
        if (nodeInGraph(contigKey))
        {
            return contigs.at(contigKey);
        }
        return nullptr;
    }

    // if present return otherwise create new one and return
    // always create pairs, first is regular, second complement
    pair<Node *, Node *> getOrInitialize(string id, int length, Type type)
    {
        Node *regular = getNode(id);
        Node *complement = getNode(id + CONTIG_SUFFIX);

        // both already exist
        if (regular != nullptr && complement != nullptr)
        {
            return make_pair(regular, complement);
        }

        // create both
        regular = new Node(
            type,
            id,
            length);

        complement = new Node(
            type,
            id,
            length,
            regular,
            true);

        regular->complement = complement;

        addNode(regular);
        addNode(complement);

        return make_pair(regular, complement);
    }
};

vector<string> split(const string str, const string regex_str)
{
    regex reg(regex_str);
    vector<string> list(sregex_token_iterator(str.begin(), str.end(), reg, -1),
                        sregex_token_iterator());
    return list;
}

Type getSequenceTypeFromName(string name)
{
    // contigs start with ctg or Ctg maybe CTG
    // checked in test data
    string label = name.substr(0, 3);
    if (label == "ctg" || label == "Ctg" || label == "CTG")
    {
        return CONTIG;
    }
    return READ;
}

// fake to string for edge, for testing purposes
ostream &operator<<(std::ostream &strm, const Edge &e)
{
    return strm << "Edge(" << e.querySequenceName << ", " << e.queryStart << ", "
                << e.queryEnd << ", " << e.relativeStrand << ", " << e.targetSequenceName
                << ", " << e.targetStart << ", " << e.targetEnd << ", " << e.numberOfMatches
                << ", " << e.alignmentBlockLengt << ", " << e.mappingQuality << ")";
}

Graph readPafFile(string filename)
{
    ifstream inFile;
    inFile.open(filename);

    string ln;
    Graph graph = Graph();

    int num = 0;

    while (getline(inFile, ln))
    {
        vector<string> spllitedLine = split(ln, "\t");

        string querySequenceName = spllitedLine[0];
        int querySequenceLength = stoi(spllitedLine[1]);

        char relativeStrand = spllitedLine[4][0];

        pair<Node *, Node *> queryNodes = graph.getOrInitialize(
            querySequenceName,
            querySequenceLength,
            getSequenceTypeFromName(querySequenceName));

        string targetSequenceName = spllitedLine[5];
        int targetSequenceLength = stoi(spllitedLine[6]);

        pair<Node *, Node *> targetNodes = graph.getOrInitialize(
            targetSequenceName,
            targetSequenceLength,
            getSequenceTypeFromName(targetSequenceName));

        int startQueryIndex = stoi(spllitedLine[2]);
        int endQUeryIndex = stoi(spllitedLine[3]);

        int startTargetIndex = stoi(spllitedLine[7]);
        int endTargetIndex = stoi(spllitedLine[8]);

        // reverse indexes
        // add edges beetwen regular and complement
        if (relativeStrand == '-')
        {
            int startTargetIndexHelper = targetSequenceLength - 1 - endTargetIndex;
            int endTargetIndexHepler = targetSequenceLength - 1 - startTargetIndex;

            Edge *edge1 = new Edge(
                querySequenceName, targetSequenceName + CONTIG_SUFFIX, startQueryIndex,
                endQUeryIndex, relativeStrand, startTargetIndexHelper,
                endTargetIndexHepler, stoi(spllitedLine[9]), stoi(spllitedLine[10]),
                stoi(spllitedLine[11]));

            queryNodes.first->addOverlap(edge1);
            targetNodes.second->addOverlap(edge1);

            int startQueryIndexHelper = querySequenceLength - 1 - endQUeryIndex;
            int endQueryIndexHepler = querySequenceLength - 1 - startQueryIndex;

            Edge *edge2 = new Edge(
                querySequenceName + CONTIG_SUFFIX, targetSequenceName, startQueryIndexHelper,
                endQueryIndexHepler, relativeStrand, startTargetIndex,
                endTargetIndex, stoi(spllitedLine[9]), stoi(spllitedLine[10]),
                stoi(spllitedLine[11]));

            queryNodes.second->addOverlap(edge2);
            targetNodes.first->addOverlap(edge2);
        }
        // no index reversing, add edges beetwen regulars
        else
        {
            Edge *edge1 = new Edge(
                querySequenceName, targetSequenceName, startQueryIndex,
                endQUeryIndex, relativeStrand, startTargetIndex,
                endTargetIndex, stoi(spllitedLine[9]), stoi(spllitedLine[10]),
                stoi(spllitedLine[11]));

            queryNodes.first->addOverlap(edge1);
            targetNodes.first->addOverlap(edge1);

            Edge *edge2 = new Edge(
                querySequenceName + CONTIG_SUFFIX, targetSequenceName + CONTIG_SUFFIX,
                querySequenceLength - 1 - endQUeryIndex, querySequenceLength - 1 - startQueryIndex,
                relativeStrand, targetSequenceLength - 1 - endTargetIndex, targetSequenceLength - 1 - startTargetIndex,
                stoi(spllitedLine[9]), stoi(spllitedLine[10]),
                stoi(spllitedLine[11]));

            queryNodes.second->addOverlap(edge2);
            targetNodes.second->addOverlap(edge2);
        }
    }

    inFile.close();
    return graph;
}

int main(int argc, char const *argv[])
{
    string fileName = "overlaps.paf";

    cout << "start" << endl;

    Graph g = readPafFile(fileName);

    cout << "end" << endl;

    cout << "num nodes: " << g.nodes.size() << endl;
    cout << "num contigs: " << g.contigs.size() << endl;

    return 0;
}
