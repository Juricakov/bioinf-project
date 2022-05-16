#include "heuristic/extensionScore.h"
#include "heuristic/overlapScore.h"
#include "heuristic/monteCarlo.h"
#include "pathSelector.h"
#include "pathGenerator.h"
#include "sequenceGenerator.h"
#include "pafParser.h"
#include <map>

int main()
{

    // validate files exist
    // create fasta reader / writer / merger ...
    // call paf parser to generate graph
    // get paths from path generator
    // perform path selection
    // create DNA sequence using merger
    // write sequence to fasta file

    // vector<string> pafFileNames{"read-read.paf", "contig-read.paf"};
    // pair<string, string> fastaFileNames{"readings.fasta", "contigs.fasta"};
    vector<string> pafFileNames{"overlaps1.paf", "overlaps2.paf"};
    pair<string, string> fastaFileNames{"ecoli_test_reads.fasta", "ecoli_test_contigs.fasta"};

    Graph g = PafParser::readPafFile(pafFileNames, fastaFileNames);

    cout << "num nodes: " << g.nodes.size() << endl;
    cout << "num contigs: " << g.contigs.size() << endl
         << endl;

    PathSelector selector;

    // key is (start, end)
    // this is needed to send appropriate paths to buckets
    map<pair<string, string>, vector<Path *>> allPaths;

    // add all paths, stored in map with start and end nodes
    for (auto contg : g.contigs)
    {
        // todo add same for mc

        Heuristic *hExtension = new ExtensionScoreHeuristic(contg.second->overlaps, g.nodes);
        auto pathExtension = PathGenerator::generate(contg.second, hExtension, g.nodes);

        Heuristic *hOverlap = new OverlapScoreHeuristic(contg.second->overlaps, g.nodes);
        auto pathOverlap = PathGenerator::generate(contg.second, hOverlap, g.nodes);

        for (int i = 0; i < max(pathExtension.size(), pathOverlap.size()); i++)
        {
            if (i < pathExtension.size())
            {
                auto pathEnd = pathExtension.at(i)->getEnd(g.contigs)->id;
                auto key = make_pair(contg.second->id, pathEnd);

                vector<Path *> previous;

                if (allPaths.find(key) != allPaths.end())
                {
                    previous = allPaths.at(key);
                }

                previous.push_back(pathExtension.at(i));
                allPaths[make_pair(contg.second->id, pathEnd)] = previous;
            }

            if (i < pathOverlap.size())
            {
                auto pathEnd = pathOverlap.at(i)->getEnd(g.contigs)->id;
                auto key = make_pair(contg.second->id, pathEnd);

                vector<Path *> previous;

                if (allPaths.find(key) != allPaths.end())
                {
                    previous = allPaths.at(key);
                }

                previous.push_back(pathOverlap.at(i));
                allPaths[make_pair(contg.second->id, pathEnd)] = previous;
            }
        }
    }

    map<pair<string, string>, Path *> selectedPaths;

    for (auto paths : allPaths)
    {
        selectedPaths[paths.first] = selector.pick(paths.second);
        cout << paths.first.first << " " << paths.first.second << endl;
    }

    /***
    Possible scenario:
    paths : c1 -> c2
            c1 -> c3
            c2 -> c3
    how to select proper?
    if i remeber correctly multiple sequences can be generated, no selecting????
    ***/

    // writing is currently done in command line with a > res.txt, will change after testing
    // cout << SequenceGenerator::generate(wholePath, g.nodes);
    return 0;
}
