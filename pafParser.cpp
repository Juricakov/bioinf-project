#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <regex>
#include "pafParser.h"
#include "fasta/reader.h"
#include "fastq/reader.h"

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

ostream &operator<<(std::ostream &strm, const Edge &e)
{
    return strm << "Edge(" << e.querySequenceName << ", " << e.queryStart << ", "
                << e.queryEnd << ", " << e.relativeStrand << ", " << e.targetSequenceName
                << ", " << e.targetStart << ", " << e.targetEnd << ")";
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

        int cnt = 0;

        while (getline(inFile, ln))
        {
            cnt++;
            if (cnt % 100000 == 0)
            {
                cout << "still alive " << cnt << endl;
            }

            // space for test files
            vector<string> spllitedLine = split(ln, "\t");

            string querySequenceName = spllitedLine[0];

            int querySequenceLength = stoi(spllitedLine[1]);

            char relativeStrand = spllitedLine[4][0];

            string targetSequenceName = spllitedLine[5];
            int targetSequenceLength = stoi(spllitedLine[6]);

            // somehow happens in read-read paf file
            if (targetSequenceName == querySequenceName)
            {
                continue;
            }

            int startQueryIndex = stoi(spllitedLine[2]);
            int endQueryIndex = stoi(spllitedLine[3]);

            int startTargetIndex = stoi(spllitedLine[7]);
            int endTargetIndex = stoi(spllitedLine[8]);

            pair<Node *, Node *> queryNodes = graph.getOrInitialize(
                querySequenceName,
                getSequenceTypeFromName(querySequenceName));

            pair<Node *, Node *> targetNodes = graph.getOrInitialize(
                targetSequenceName,
                getSequenceTypeFromName(targetSequenceName));

            // reverse indexes
            // add edges beetwen regular and complement
            if (relativeStrand == '-')
            {
                int startTargetIndexHelper = targetSequenceLength - endTargetIndex;
                int endTargetIndexHepler = targetSequenceLength - startTargetIndex;

                Edge *edge1 = new Edge(
                    querySequenceName, targetSequenceName + COMPLEMENT_SUFFIX, startQueryIndex,
                    endQueryIndex, relativeStrand, startTargetIndexHelper,
                    endTargetIndexHepler, stoi(spllitedLine[10]), querySequenceLength, targetSequenceLength);

                int startQueryIndexHelper = querySequenceLength - endQueryIndex;
                int endQueryIndexHepler = querySequenceLength - startQueryIndex;

                Edge *edge2 = new Edge(
                    querySequenceName + COMPLEMENT_SUFFIX, targetSequenceName, startQueryIndexHelper,
                    endQueryIndexHepler, relativeStrand, startTargetIndex,
                    endTargetIndex, stoi(spllitedLine[10]), querySequenceLength, targetSequenceLength);

                // check if extends
                int baseCountAfterOverlapOnQuerySeq = querySequenceLength - endQueryIndex;
                int baseCountAfterOverlapOnTargetSeq = targetSequenceLength - endTargetIndexHepler;

                // do not create edge or nodes if overlap does not extend the sequence
                // no index reversing, add edges beetwen regulars
            }
            else
            {
                Edge *edge1 = new Edge(
                    querySequenceName, targetSequenceName, startQueryIndex,
                    endQueryIndex, relativeStrand, startTargetIndex,
                    endTargetIndex, stoi(spllitedLine[10]), querySequenceLength, targetSequenceLength);

                Edge *edge2 = new Edge(
                    querySequenceName + COMPLEMENT_SUFFIX, targetSequenceName + COMPLEMENT_SUFFIX,
                    querySequenceLength - endQueryIndex, querySequenceLength - startQueryIndex,
                    relativeStrand, targetSequenceLength - endTargetIndex, targetSequenceLength - startTargetIndex,
                    stoi(spllitedLine[10]), querySequenceLength, targetSequenceLength);

                // check if extends
                int baseCountAfterOverlapOnQuerySeq = querySequenceLength - endQueryIndex;
                int baseCountAfterOverlapOnTargetSeq = targetSequenceLength - endTargetIndex;

                if (baseCountAfterOverlapOnTargetSeq > baseCountAfterOverlapOnQuerySeq)
                {
                    queryNodes.first->addOverlap(edge1);
                    targetNodes.second->addOverlap(edge2->flipQueryAndTarget());
                }

                else
                {
                    targetNodes.first->addOverlap(edge1->flipQueryAndTarget());
                    queryNodes.second->addOverlap(edge2);
                }
            }
        }
        inFile.close();
    }
    vector<shared_ptr<NamedSequnce>> s1;
    vector<shared_ptr<NamedSequnce>> s2;

    if (fastaFilenames.first.find("fastq") != string::npos)
    {
        cout << "fastq" << endl;
        s1 = FASTQReader::read(fastaFilenames.first);
    }
    else
    {
        s1 = FASTAReader::read(fastaFilenames.first);
    }

    if (fastaFilenames.second.find("fastq") != string::npos)
    {
        cout << "fastq" << endl;
        s2 = FASTQReader::read(fastaFilenames.second);
    }
    else
    {
        s2 = FASTAReader::read(fastaFilenames.second);
    }

    s1.insert(s1.end(), s2.begin(), s2.end());
    graph.addSequences(s1);
    return graph;
}
