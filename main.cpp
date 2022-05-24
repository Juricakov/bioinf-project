#include "heuristic/extensionScore.h"
#include "heuristic/overlapScore.h"
#include "heuristic/monteCarlo.h"
#include "pathSelector.h"
#include "pathGenerator.h"
#include "sequenceGenerator.h"
#include "pafParser.h"
#include "pathMerger.h"
#include "unordered_set"
#include "fasta/writer.h"
#include <chrono>

int Path::current_id = 0;

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    if (argc != 6)
    {
        throw runtime_error("Missing arguments");
    }

    vector<string> pafFileNames{argv[1], argv[2]};
    pair<string, string> fastaFileNames{argv[3], argv[4]};
    string resultFilename = argv[5];

    Graph g = PafParser::readPafFile(pafFileNames, fastaFileNames);

    cout << "num nodes: " << g.nodes.size() << endl;
    cout << "num contigs: " << g.contigs.size() << endl;

    SequenceGenerator generator;
    PathSelector selector(generator);

    Node *startNode;
    unordered_map<string, Path *> selectedPathForNode;
    vector<Path *> pathsForFinalPath;

    int maxLength = 0;

    for (auto contig : g.contigs)
    {
        startNode = contig.second;

        vector<Path *> fullPath;
        unordered_set<string> visited;
        // stop after pick or ignore earlier?

        visited.insert(startNode->id);

        int cnt = 0;
        // max path connects all contigs (/2 because of complements)
        while (startNode != nullptr && cnt < g.contigs.size() / 2)
        {
            Path *selectedPath;
            // new paths needs to be created
            if (selectedPathForNode.find(startNode->key) == selectedPathForNode.end())
            {
                cout << endl
                     << startNode->key << endl;

                Heuristic *hExtension = new ExtensionScoreHeuristic(startNode->getOverlaps());
                auto pathsExtension = PathGenerator::generate(startNode, hExtension, g.nodes, 20);

                cout << "overlap" << endl;

                Heuristic *hOverlap = new OverlapScoreHeuristic(startNode->getOverlaps());
                auto pathsOverlap = PathGenerator::generate(startNode, hOverlap, g.nodes, 20);

                cout << "mc" << endl;

                Heuristic *hMonteCarlo = new MonteCarloHeuristic(startNode->getOverlaps());
                auto pathsMonteCarlo = PathGenerator::generate(startNode, hMonteCarlo, g.nodes, 40);

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
                if (selectedPath != nullptr)
                {
                    cout << "after pick " << selectedPath->getEndNodeName() << endl;
                }
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

            cout << selectedPath->getStartNodeName() << " " << selectedPath->getEndNodeName() << endl;

            fullPath.push_back(selectedPath);
            cnt++;
        }

        cout << "num of connected contigs " << cnt << endl;

        // if max number of contigs connected stop search
        if (fullPath.size() == (g.contigs.size() - 1) / 2)
        {
            pathsForFinalPath.clear();
            std::copy(fullPath.begin(), fullPath.end(), std::back_inserter(pathsForFinalPath));
            break;
        }

        if (cnt > maxLength)
        {
            pathsForFinalPath.clear();
            std::copy(fullPath.begin(), fullPath.end(), std::back_inserter(pathsForFinalPath));
            maxLength = cnt;
        }
    }

    cout << "before merger " << pathsForFinalPath.size() << endl;

    auto finalPath = PathMerger::merge(pathsForFinalPath, g.nodes);

    cout << "final path " << finalPath->getStartNodeName() << " " << finalPath->getEndNodeName() << endl;

    auto finalSequence = generator.generate(finalPath, g.nodes);

    cout << finalSequence.length() << endl;

    shared_ptr<NamedSequnce> namedFinal = shared_ptr<NamedSequnce>(new NamedSequnce("final sequence", finalSequence));
    FASTAWriter::write(resultFilename, {namedFinal});

    auto stop = std::chrono::high_resolution_clock::now();

    cout << "DONE" << endl;
    cout << "time: " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds";
    return 0;
}
