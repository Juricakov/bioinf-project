#include <string>
#include <vector>
#include <memory>
#include "../namedSequence.h"

class FASTQReader
{
private:
    FASTQReader();

public:
    static std::vector<std::shared_ptr<NamedSequnce>> read(std::string path);
};