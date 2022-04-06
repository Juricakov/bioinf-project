#include <string>

class FASTAReading
{
private:
    std::string name, sequence;

public:
    FASTAReading(std::string name, std::string sequence);
    std::string getName();
    std::string getSequence();
};