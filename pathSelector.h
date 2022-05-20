#include "path.h"
#include <vector>
#include "Node.h"
#include "sequenceGenerator.h"

const int defaultBucketWindowSize = 1000;
const int defaultMaxSingleBucketLenRange = 10000;

class PathSelector
{
private:
    int bucketWindow;
    int maxSingleBucketLenRange;
    SequenceGenerator sequenceGenerator;

public:
    PathSelector(SequenceGenerator sequenceGenerator, int bucketWindow, int maxSingleBucketLenRange)
    {
        this->bucketWindow = bucketWindow;
        this->maxSingleBucketLenRange = maxSingleBucketLenRange;
        this->sequenceGenerator = sequenceGenerator;
    }

    PathSelector(SequenceGenerator sequenceGenerator)
    {
        PathSelector(sequenceGenerator, defaultBucketWindowSize, defaultMaxSingleBucketLenRange);
    }

    Path *pick(std::vector<Path> paths, std::unordered_map<string, Node *> lookup);
};