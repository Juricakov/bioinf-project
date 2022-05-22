#pragma once
#include "path.h"
#include <vector>
#include "Node.h"
#include "sequenceGenerator.h"

const int defaultBucketWindowSize = 1000;
const int defaultMaxSingleBucketLenRange = 10000;

class PathSelector
{
private:
    // todo for some reason constructor does not work
    // fot ints only, sequence generator ok?
    int bucketWindow = defaultBucketWindowSize;
    int maxSingleBucketLenRange = defaultMaxSingleBucketLenRange;
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

    Path *pick(std::vector<Path *> paths, std::unordered_map<string, Node *> lookup);
};