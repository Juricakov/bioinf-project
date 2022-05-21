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

    // hand made test example, replace "\t" with " " in paf parser
    // vector<string> pafFileNames{"read-read.paf", "contig-read.paf"};
    // pair<string, string> fastaFileNames{"readings.fasta", "contigs.fasta"};

    // ecoli test data files
    vector<string> pafFileNames{"overlaps1.paf", "overlaps2.paf"};
    pair<string, string> fastaFileNames{"ecoli_test_reads.fasta", "ecoli_test_contigs.fasta"};

    // cjejuni data
    // vector<string> pafFileNames{"overlapsCjejuniCR.paf", "overlapsCjejuniRR.paf"};
    // pair<string, string> fastaFileNames{"CJejuni - reads.fastq", "CJejuni - contigs.fasta"};

    Graph g = PafParser::readPafFile(pafFileNames, fastaFileNames);

    cout << "num nodes: " << g.nodes.size() << endl;
    cout << "num contigs: " << g.contigs.size() << endl
         << endl;

    // complex main not needed, all paths from same node are sent to selector
    // all paths are sent to merger
    // it should not happen that multiple paths end in the same node (probability)
    // not possible to have multiple paths starting from the same node

    PathSelector selector;

    vector<Path *> allPaths;

    // duplicate entries in paf file for read read ????????
    // set does not solve somehow

    for (auto contig : g.contigs)
    {
        // todo add same for mc

        // Heuristic *hExtension = new ExtensionScoreHeuristic(contig.second->overlaps, g.nodes);
        // auto pathsExtension = PathGenerator::generate(contg.second, hExtension, g.nodes);

        // Heuristic *hOverlap = new OverlapScoreHeuristic(contig.second->overlaps, g.nodes);
        // auto pathsOverlap = PathGenerator::generate(contig.second, hOverlap, g.nodes);

        // only test, remove
        if (contig.first != "ctg1")
        {
            continue;
        }

        vector<Edge*> v;
        for (auto it = contig.second->overlaps.begin(); it != contig.second->overlaps.end(); ++it)
        {
            v.push_back(it->second);
        }

        Heuristic *hMonteCarlo = new MonteCarloHeuristic(v, g.nodes);
        auto pathsMonteCarlo = PathGenerator::generate(contig.second, hMonteCarlo, g.nodes);

        // allPaths.push_back(selector.pick(pathsMonteCarlo));
    }

    // writing is currently done in command line with a > res.txt, will change after testing
    // cout << SequenceGenerator::generate(wholePath, g.nodes);
    return 0;
}
