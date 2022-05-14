#include <string>
#include <vector>
#include <memory>
#include "namedSequence.h"

class FASTAWriter
{
private:
    FASTAWriter();

public:
    static void write(std::string path, std::vector<std::shared_ptr<NamedSequnce>> readings);
};