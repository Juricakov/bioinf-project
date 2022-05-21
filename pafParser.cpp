#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <regex>
#include "pafParser.h"
#include "fasta/reader.h"

using namespace std;

vector<string> PafParser::split(const string str, const string regex_str)
{
    regex reg(regex_str);
    vector<string> list(sregex_token_iterator(str.begin(), str.end(), reg, -1),
                        sregex_token_iterator());
    return list;
}

Type PafParser::getSequenceTypeFromName(string name)
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

Graph PafParser::readPafFile(vector<string> pafFilenames, pair<string, string> fastaFilenames)
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

            // somehow happens in read-read paf file
            if (targetSequenceName == querySequenceName){
                continue;
            }

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
                int startTargetIndexHelper = targetSequenceLength - endTargetIndex;
                int endTargetIndexHepler = targetSequenceLength - startTargetIndex;

                Edge *edge1 = new Edge(
                    querySequenceName, targetSequenceName + COMPLEMENT_SUFFIX, startQueryIndex,
                    endQUeryIndex, relativeStrand, startTargetIndexHelper,
                    endTargetIndexHepler, stoi(spllitedLine[10]));

                queryNodes.first->addOverlap(edge1);
                targetNodes.second->addOverlap(edge1->flipQueryAndTarget());

                int startQueryIndexHelper = querySequenceLength - endQUeryIndex;
                int endQueryIndexHepler = querySequenceLength - startQueryIndex;

                Edge *edge2 = new Edge(
                    querySequenceName + COMPLEMENT_SUFFIX, targetSequenceName, startQueryIndexHelper,
                    endQueryIndexHepler, relativeStrand, startTargetIndex,
                    endTargetIndex, stoi(spllitedLine[10]));

                queryNodes.second->addOverlap(edge2);
                targetNodes.first->addOverlap(edge2->flipQueryAndTarget());
            }
            // no index reversing, add edges beetwen regulars
            else
            {
                Edge *edge1 = new Edge(
                    querySequenceName, targetSequenceName, startQueryIndex,
                    endQUeryIndex, relativeStrand, startTargetIndex,
                    endTargetIndex, stoi(spllitedLine[10]));

                queryNodes.first->addOverlap(edge1);
                targetNodes.first->addOverlap(edge1->flipQueryAndTarget());

                Edge *edge2 = new Edge(
                    querySequenceName + COMPLEMENT_SUFFIX, targetSequenceName + COMPLEMENT_SUFFIX,
                    querySequenceLength - endQUeryIndex, querySequenceLength - startQueryIndex,
                    relativeStrand, targetSequenceLength - endTargetIndex, targetSequenceLength - startTargetIndex,
                    stoi(spllitedLine[10]));

                queryNodes.second->addOverlap(edge2);
                targetNodes.second->addOverlap(edge2->flipQueryAndTarget());
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
