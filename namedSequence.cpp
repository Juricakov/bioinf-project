#include <string>
#include <iostream>
#include "namedSequence.h"
#include <algorithm>

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

char getComplementaryBase(char c)
{
    if (c == 'A')
        return 'T';
    if (c == 'T')
        return 'A';
    if (c == 'G')
        return 'C';
    if (c == 'C')
        return 'G';

    throw std::invalid_argument("Can not complement base");
}

std::string NamedSequnce::complement()
{
    std::string comp = "";

    for (auto c : this->sequence)
    {
        comp += getComplementaryBase(c);
    }

    std::reverse(comp.begin(), comp.end());

    return comp;
}