// take overlap data, path, take base sequences and merge them

// ctg1 -> reading1 -> reading2 -> reading 3 -> ctg3
// some overlap data
// ctg1 = CTGAAA
// reading 1 = AACCCTG
// reading 2 = CTGAAATC
// reading 3 = ATCGCC
// ctg3 = GCCAAA

// output = CTGACCAAGCCAAA
#include <string>
#include "path.h"
#include "vector"

// TODO Jurica implement
class PathMerger
{
public:
    static Path *merge(std::vector<Path *> paths);
};