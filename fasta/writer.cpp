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

int main()
{
    std::vector<std::string> names{"a", "b", "c"};
    std::vector<std::string> sequences{"AAA", "BBB", "CCC"};
    std::vector<std::shared_ptr<NamedSequnce>> readings;

    for (int i = 0; i < names.size(); i++)
    {
        std::shared_ptr<NamedSequnce> reading(new NamedSequnce(names[i], sequences[i]));
        readings.push_back(reading);
    }

    FASTAWriter::write("test.fasta", readings);
}