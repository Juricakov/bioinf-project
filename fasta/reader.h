#include <string>
#include <vector>
#include <memory>
#include "../namedSequence.h"

class FASTAReader
{
private:
    FASTAReader();

public:
    static std::vector<std::shared_ptr<NamedSequnce>> read(std::string path);
};