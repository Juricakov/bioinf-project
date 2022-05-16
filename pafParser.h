#include <string>
#include <vector>
#include "Global.h"
#include "Graph.h"

using namespace std;

// class for generating graph from paf files
class PafParser
{
private:
    static vector<string> split(const string str, const string regex_str);
    static Type getSequenceTypeFromName(string name);

public:
    static Graph readPafFile(vector<string> pafFilenames, pair<string, string> fastaFilenames);
};
