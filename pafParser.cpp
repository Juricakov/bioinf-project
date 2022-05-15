#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <regex>
#include "Global.h"
#include "Graph.h"
#include "fasta/reader.h"

using namespace std;

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
        return Type::CONTIG;
    }
    return Type::READ;
}

// fake to string for edge, for testing purposes
ostream &operator<<(std::ostream &strm, const Edge &e)
{
    return strm << "Edge(" << e.querySequenceName << ", " << e.queryStart << ", "
                << e.queryEnd << ", " << e.relativeStrand << ", " << e.targetSequenceName
                << ", " << e.targetStart << ", " << e.targetEnd << ", "
                << e.alignmentBlockLength << ", "
                << ")";
}

Graph readPafFile(vector<string> pafFilenames, pair<string, string> fastaFilenames)
{
    Graph graph = Graph();
    ifstream inFile;

    for (auto file : pafFilenames)
    {

        inFile.open(file);

        string ln;

        cout << file << endl;

        while (getline(inFile, ln))
        {
            // space for test files
            vector<string> spllitedLine = split(ln, " ");

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
                    querySequenceName, targetSequenceName + COMPLEMENT_SUFFIX, startQueryIndex,
                    endQUeryIndex, relativeStrand, startTargetIndexHelper,
                    endTargetIndexHepler, stoi(spllitedLine[10]));

                queryNodes.first->addOverlap(edge1);
                targetNodes.second->addOverlap(edge1);

                int startQueryIndexHelper = querySequenceLength - 1 - endQUeryIndex;
                int endQueryIndexHepler = querySequenceLength - 1 - startQueryIndex;

                Edge *edge2 = new Edge(
                    querySequenceName + COMPLEMENT_SUFFIX, targetSequenceName, startQueryIndexHelper,
                    endQueryIndexHepler, relativeStrand, startTargetIndex,
                    endTargetIndex, stoi(spllitedLine[10]));

                queryNodes.second->addOverlap(edge2);
                targetNodes.first->addOverlap(edge2);
            }
            // no index reversing, add edges beetwen regulars
            else
            {
                Edge *edge1 = new Edge(
                    querySequenceName, targetSequenceName, startQueryIndex,
                    endQUeryIndex, relativeStrand, startTargetIndex,
                    endTargetIndex, stoi(spllitedLine[10]));

                queryNodes.first->addOverlap(edge1);
                targetNodes.first->addOverlap(edge1);

                Edge *edge2 = new Edge(
                    querySequenceName + COMPLEMENT_SUFFIX, targetSequenceName + COMPLEMENT_SUFFIX,
                    querySequenceLength - 1 - endQUeryIndex, querySequenceLength - 1 - startQueryIndex,
                    relativeStrand, targetSequenceLength - 1 - endTargetIndex, targetSequenceLength - 1 - startTargetIndex,
                    stoi(spllitedLine[10]));

                queryNodes.second->addOverlap(edge2);
                targetNodes.second->addOverlap(edge2);
            }
        }
        inFile.close();
    }

    auto sequenceReads = FASTAReader::read(fastaFilenames.first);
    auto sequenceContigs = FASTAReader::read(fastaFilenames.second);
    sequenceReads.insert(sequenceReads.end(), sequenceContigs.begin(), sequenceContigs.end());

    graph.addSequences(sequenceReads);

    return graph;
}

int main(int argc, char const *argv[])
{

    vector<string> pafFileNames{"read-read.paf", "contig-read.paf"};
    pair<string, string> fastaFileNames{"readings.fasta", "contigs.fasta"};
    // vector<string> fileNames{"overlaps1.paf", "overlaps2.paf"};

    Graph g = readPafFile(pafFileNames, fastaFileNames);

    cout << "num nodes: " << g.nodes.size() << endl;
    cout << "num contigs: " << g.contigs.size() << endl << endl;


    for (auto node : g.nodes)
    {
        cout << node.first << endl;
        cout << node.second->sequence << endl;
    }

    return 0;
}