#include <string>
#include <iostream>
#include "namedSequence.h"

NamedSequnce::NamedSequnce(std::string name, std::string sequence)
{
    this->name = name;
    this->sequence = sequence;
}

std::ostream &operator<<(std::ostream &stream, const NamedSequnce &reading)
{
    return stream << ">" << reading.name << std::endl
                  << reading.sequence;
}