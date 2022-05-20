#include "pathSelector.h"
#include <algorithm>
#include <stdexcept>
#include <bits/stdc++.h>
#include <cmath>

std::vector<std::vector<Path>> partitionIntoBuckets(std::vector<Path> sortedPaths, std::unordered_map<int, int> pathLengthsById, int bucketWindowSize)
{
    std::vector<std::vector<Path>> buckets;

    std::vector<Path> bucket;
    int bucketMin = sortedPaths.front().size();
    int bucketMax = bucketMin + bucketWindowSize;

    for (auto path : sortedPaths)
    {
        if (bucketMin <= path.size() && path.size() <= bucketMax)
        {
            bucket.push_back(path);
            continue;
        }

        buckets.push_back(bucket);
        bucket = std::vector<Path>();

        bucket.push_back(path);
        bucketMin = path.size();
        bucketMax = bucketMin + bucketWindowSize;
    }

    buckets.push_back(bucket);
    return buckets;
}

std::pair<Path *, int> getGroupConsensusPathAndValidPathNumber(std::vector<Path> bucket, std::unordered_map<int, int> pathLengthsById)
{
    // calculate group length range, if its too big, its a complex repeat, so we do not return a sequence for this group.
    int maxLen = -1;
    int minLen = INT_MAX;
    for (auto path : bucket)
    {
        int len = pathLengthsById.at(path.getId());
        if (len > maxLen)
        {
            maxLen = len;
        }
        if (len < minLen)
        {
            minLen = len;
        }
    }

    auto range = maxLen - minLen;
    // complex repeat
    if (range > 100000)
    {
        return std::pair<Path *, int>(nullptr, 0);
    }

    // discard the sorter paths
    std::vector<Path> options;
    std::copy_if(bucket.begin(), bucket.end(), std::back_inserter(options), [pathLengthsById, maxLen](Path p)
                 { return pathLengthsById.at(p.getId()) > maxLen / 2; });

    // we will use path that is most similar to other paths in length.
    // the number of similar paths is called the valid path number.
    int similarityThreshold = 100;
    Path mostSimilarPaths;
    int similarPathCount = INT_MAX;

    for (auto path1 : options)
    {
        int similarCount = 0;
        int len1 = pathLengthsById[path1.getId()];
        for (auto path2 : options)
        {
            if (path1.getId() == path2.getId())
                continue;

            int len2 = pathLengthsById[path2.getId()];
            if (abs(len1 - len2) < similarityThreshold)
            {
                similarCount++;
            }
        }

        if (similarCount > similarPathCount)
        {
            mostSimilarPaths = path1;
            similarPathCount = similarCount;
        }
    }

    return std::pair<Path *, int>(&mostSimilarPaths, similarPathCount);
}

Path *PathSelector::pick(std::vector<Path> paths, std::unordered_map<string, Node *> lookup)
{

    std::unordered_map<int, int> pathLengthsById;
    std::string startNodeName = paths.front().getStartNodeName();

    int minPathLength = INT_MAX;
    int maxPathLength = -1;

    for (auto path : paths)
    {
        auto len = this->sequenceGenerator.generate(&path, lookup).length();
        pathLengthsById[path.getId()] = len;
        if (len > maxPathLength)
        {
            maxPathLength = len;
        }
        if (len < minPathLength)
        {
            minPathLength = len;
        }

        if (startNodeName != path.getStartNodeName())
        {
            throw std::invalid_argument("Paths must all have same start node name");
        }
    }

    // we need to split all paths into buckets.
    std::vector<std::vector<Path>> buckets;
    // bucketLengthSums[i] = sum(len(buckets[i]));
    std::vector<int> bucketLengthSums;

    // If the path lengths are distributed within a small range,
    // all the paths are grouped into one group
    int pathLenRange = maxPathLength - minPathLength;
    if (pathLenRange <= this->maxSingleBucketLenRange)
    {
        std::vector<std::vector<Path>> buckets;
        int lenSum = 0;
        for (auto path : paths)
        {
            lenSum += pathLengthsById[path.getId()];
        }
        buckets.push_back(paths);
        bucketLengthSums.push_back(lenSum);
    }
    else
    {
        // Otherwise, the paths are divided into separate groups using a small window (1kb)
        std::vector<Path> sortedPaths(paths.begin(), paths.end());
        std::sort(sortedPaths.begin(), sortedPaths.end(), [pathLengthsById](Path a, Path b)
                  { return pathLengthsById.at(a.getId()) < pathLengthsById.at(b.getId()); });

        auto buckets = partitionIntoBuckets(sortedPaths, pathLengthsById, this->bucketWindow);

        // peak window is the one with the highest length sum
        int peakBucketLengthSum = 0;
        int peakBucketHighestLength;

        // valley window is the one with the smallest length sum
        int valleyBucketLengthSum = INT_MAX;
        int valleyBucketLowestLength;

        for (auto bucket : buckets)
        {
            int bucketLenSum = 0;
            int lowestLen = INT_MAX;
            int highestLen = 0;

            for (auto path : bucket)
            {
                int len = pathLengthsById.at(path.getId());
                bucketLenSum += len;
                if (len < lowestLen)
                {
                    lowestLen = len;
                }
                if (len > highestLen)
                {
                    highestLen = len;
                }
            }
            // populate this because we get it for free here
            bucketLengthSums.push_back(bucketLenSum);
            if (bucketLenSum > peakBucketLengthSum)
            {
                peakBucketLengthSum = bucketLenSum;
                peakBucketHighestLength = highestLen;
            }

            if (bucketLenSum < valleyBucketLengthSum)
            {
                valleyBucketLengthSum = bucketLenSum;
                valleyBucketLowestLength = lowestLen;
            }
        }

        // if the lowest length in valley is less than 90% of the highest length in the peak,
        // then the path length of the lowest length in valley bucket is used to divide the original paths into buckets
        if (valleyBucketLowestLength < (0.9 * peakBucketHighestLength))
        {
            buckets = partitionIntoBuckets(sortedPaths, pathLengthsById, valleyBucketLowestLength);
            // dont forget to update bucket lengths
            bucketLengthSums.clear();
            for (auto bucket : buckets)
            {
                int sum = 0;
                for (auto path : bucket)
                {
                    sum += pathLengthsById[path.getId()];
                }
                bucketLengthSums.push_back(sum);
            }
        }
    }

    // get representative path for each group -> see getGroupConsensusPathAndValidPathNumber
    std::vector<std::pair<Path *, int>> representativePathAndValidPathCount;

    std::transform(buckets.begin(), buckets.end(), std::back_inserter(representativePathAndValidPathCount), [pathLengthsById](std::vector<Path> bucket) -> std::pair<Path *, int>
                   { return getGroupConsensusPathAndValidPathNumber(bucket, pathLengthsById); });

    // if only one group, simple outcome.
    if (representativePathAndValidPathCount.size() == 1)
    {
        return representativePathAndValidPathCount.at(0).first;
    }

    // otherwise take representative sequences of two buckets with the longest length sum
    std::pair<Path *, int> maxGroupLengthSumSequence;
    int maxGroupLengthSum = 0;
    std::pair<Path *, int> secondMaxGroupLengthSumSequence;
    int secondMaxGroupLengthSum = 0;

    for (int i = 0; i < buckets.size(); i++)
    {
        int lenSum = bucketLengthSums.at(i);
        auto sequence = representativePathAndValidPathCount.at(i);

        if (lenSum > maxGroupLengthSum)
        {
            // shift max down
            secondMaxGroupLengthSumSequence = maxGroupLengthSumSequence;
            secondMaxGroupLengthSum = maxGroupLengthSum;
            // update max
            maxGroupLengthSumSequence = sequence;
            maxGroupLengthSum = lenSum;
        }
    }

    // if valid path count in longer sequence is more than 2x in shorter sequence, then use longer sequence
    if (maxGroupLengthSumSequence.second > 2 * secondMaxGroupLengthSumSequence.second)
    {
        return maxGroupLengthSumSequence.first;
    }
    // otherwise
    return secondMaxGroupLengthSumSequence.first;
}