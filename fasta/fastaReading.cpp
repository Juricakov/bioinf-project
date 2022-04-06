#include <string>
#include "fastaReading.h"

FASTAReading::FASTAReading(std::string name, std::string sequence)
{
    name = name;
    sequence = sequence;
}
std::string FASTAReading::getName()
{
    return name;
}
std::string FASTAReading::getSequence()
{
    return sequence;
}