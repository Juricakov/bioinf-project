#include "heuristic/extensionScore.h"
#include "heuristic/overlapScore.h"
#include "heuristic/monteCarlo.h"
#include "pathSelector.h"
#include "pathGenerator.h"
#include "sequenceGenerator.h"
#include "pafParser.h"
#include "pathMerger.h"
#include "unordered_set"

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
    cout << "num contigs: " << g.contigs.size() << endl;

    // complex main not needed, all paths from same node are sent to selector
    // all paths are sent to merger
    // it should not happen that multiple paths end in the same node (probability)
    // not possible to have multiple paths starting from the same node

    SequenceGenerator generator;
    PathSelector selector(generator);

    Node *startNode;
    unordered_map<string, Path *> selectedPathForNode;
    vector<Path *> fullPath(g.contigs.size());

    for (auto contig : g.contigs)
    {
        // // start contig should not be complement
        // if (contig.second->isComplement())
        //     continue;

        startNode = contig.second;

        fullPath.clear();
        unordered_set<string> visited;
        // stop after pick or ignore earlier?

        visited.insert(startNode->id);

        int cnt = 0;
        // max path connects all contigs (/2 because of complements)
        while (startNode != nullptr && cnt < (g.contigs.size() - 1) / 2)
        {
            Path *selectedPath;
            // new paths needs to be created
            if (selectedPathForNode.find(startNode->key) == selectedPathForNode.end())
            {
                cout << endl
                     << startNode->key << endl;

                Heuristic *hExtension = new ExtensionScoreHeuristic(startNode->getOverlaps());
                auto pathsExtension = PathGenerator::generate(startNode, hExtension, g.nodes);

                cout << "overlap" << endl;

                Heuristic *hOverlap = new OverlapScoreHeuristic(startNode->getOverlaps());
                auto pathsOverlap = PathGenerator::generate(startNode, hOverlap, g.nodes);

                cout << "mc" << endl;

                Heuristic *hMonteCarlo = new MonteCarloHeuristic(startNode->getOverlaps());
                auto pathsMonteCarlo = PathGenerator::generate(startNode, hMonteCarlo, g.nodes);

                vector<Path *> pathsOneNode;

                for (int i = 0; i < max({pathsExtension.size(), pathsOverlap.size(), pathsMonteCarlo.size()}); i++)
                {
                    if (i < pathsExtension.size())
                    {
                        pathsOneNode.push_back(pathsExtension.at(i));
                    }

                    if (i < pathsOverlap.size())
                    {
                        pathsOneNode.push_back(pathsOverlap.at(i));
                    }

                    if (i < pathsMonteCarlo.size())
                    {
                        pathsOneNode.push_back(pathsMonteCarlo.at(i));
                    }
                }

                if (pathsOneNode.empty())
                {
                    selectedPathForNode[startNode->key] = nullptr;
                    break;
                }

                selectedPath = selector.pick(pathsOneNode, g.nodes);
                selectedPathForNode[startNode->key] = selectedPath;
            }
            else
            {
                selectedPath = selectedPathForNode.at(startNode->key);
            }

            if (selectedPath == nullptr)
                break;

            startNode = selectedPath->getEnd(g.nodes);

            if (visited.find(startNode->id) != visited.end())
            {
                break;
            }
            visited.insert(startNode->id);

            cout << endl
                 << selectedPath->getStartNodeName() << " " << selectedPath->getEndNodeName() << endl;

            fullPath.push_back(selectedPath);
            cnt++;
        }

        // if full path found stop search
        if (fullPath.size() == (g.contigs.size() - 1) / 2)
        {
            break;
        }
    }

    cout << "before merger" << endl;

    auto finalPath = PathMerger::merge(fullPath, g.nodes);
    auto finalSequence = generator.generate(finalPath, g.nodes);

    cout << finalSequence.length() << endl
         << endl;
    cout << finalSequence;
    return 0;
}
