#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "writer.h"

void FASTAWriter::write(std::string path, std::vector<std::shared_ptr<NamedSequnce>> readings)
{
    std::ofstream file(path);
    for (auto &reading : readings)
    {
        file << *reading << std::endl;
    }

    file.close();
}
