#include "heuristic/extensionScore.h"
#include "heuristic/overlapScore.h"
#include "heuristic/monteCarlo.h"
#include "pathSelector.h"
#include "pathGenerator.h"
#include "sequenceGenerator.h"
#include "pafParser.h"
#include "pathMerger.h"

int Path::current_id = 0;

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

    SequenceGenerator generator;
    PathSelector selector(generator);

    vector<Path *> allPaths;

    // maybe try to generate full path from each
    for (auto contig : g.contigs)
    {
        // start contig should not be complement
        if (contig.second->isComplement()) continue;

        cout << contig.first << endl;

        cout << "extension" << endl;
        Heuristic *hExtension = new ExtensionScoreHeuristic(contig.second->getOverlaps());
        auto pathsExtension = PathGenerator::generate(contig.second, hExtension, g.nodes);

        cout << "overlap" << endl;

        Heuristic *hOverlap = new OverlapScoreHeuristic(contig.second->getOverlaps());
        auto pathsOverlap = PathGenerator::generate(contig.second, hOverlap, g.nodes);

        cout << "mc" << endl;
        Heuristic *hMonteCarlo = new MonteCarloHeuristic(contig.second->getOverlaps());
        auto pathsMonteCarlo = PathGenerator::generate(contig.second, hMonteCarlo, g.nodes);

        vector<Path> pathsOneNode;

        for (int i = 0; i < max({pathsExtension.size(), pathsOverlap.size(), pathsMonteCarlo.size()}); i++)
        {
            if (i < pathsExtension.size())
            {
                pathsOneNode.push_back(*pathsExtension.at(i));
            }

            if (i < pathsOverlap.size())
            {
                pathsOneNode.push_back(*pathsOverlap.at(i));
            }

            if (i < pathsMonteCarlo.size())
            {
                pathsOneNode.push_back(*pathsMonteCarlo.at(i));
            }
        }

        if (pathsOneNode.empty())
        {
            continue;
        }

        // auto path = selector.pick(pathsOneNode, g.nodes);

        // // cout << "picker" << endl;
        // // cout << path->getId() << " " << path->getStartNodeName() << endl;

        allPaths.push_back(pathsExtension.at(0));
    }

    cout << "all paths " << allPaths.size() << endl;

    for (auto path : allPaths)
    {
        cout << "path" << endl;
        cout << path->getId() << " " << path->getStartNodeName() << " " << path->getEndNodeName() << endl;
    }

    auto finalPath = PathMerger::merge(allPaths, g.nodes);

    cout << "end merge" << endl;

    cout << finalPath->getStart(g.nodes)->key << endl;
    cout << finalPath->getEnd(g.nodes)->key << endl;
    cout << finalPath->size() << endl;

    auto finalSequence = generator.generate(finalPath, g.nodes);

    cout << finalSequence.size();

    return 0;
}
