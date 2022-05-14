#include <string>
#include <iostream>

class NamedSequnce
{
public:
    NamedSequnce(std::string name, std::string sequence);
    std::string name;
    std::string sequence;
    // TODO Jurica implement
    NamedSequnce complement();
    friend std::ostream &operator<<(std::ostream &os, const NamedSequnce &reading);
};