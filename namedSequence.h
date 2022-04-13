#include <string>
#include <iostream>

class NamedSequnce
{
public:
    NamedSequnce(std::string name, std::string sequence);
    std::string name;
    std::string sequence;
    friend std::ostream &operator<<(std::ostream &os, const NamedSequnce &reading);
};