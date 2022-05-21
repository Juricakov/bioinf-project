#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include "reader.h"

bool startsWith(std::string str, std::string prefix)
{
    return str.compare(0, prefix.length(), prefix) == 0;
}

const char *whitespace = " \t\n\r\f\v";

inline std::string &rtrim(std::string &s)
{
    s.erase(s.find_last_not_of(whitespace) + 1);
    return s;
}

inline std::string &ltrim(std::string &s)
{
    s.erase(0, s.find_first_not_of(whitespace));
    return s;
}

inline std::string &trim(std::string &s)
{
    return ltrim(rtrim(s));
}

inline bool file_exists(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

std::vector<std::shared_ptr<NamedSequnce>> FASTQReader::read(std::string path)
{
    if (!file_exists(path))
    {
        throw std::invalid_argument("File does not exist");
    }

    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::invalid_argument("Can not open file");
    }

    std::vector<std::shared_ptr<NamedSequnce>> readings;
    std::string line, name, sequence;
    bool skipNext = false;

    while (std::getline(file, line))
    {
        if (skipNext)
        {
            skipNext = false;
            continue;
        }

        line = trim(line);

        if (startsWith(line, ";") || (line.length() == 0)) // comment or empty
        {
            continue;
        }

        if (startsWith(line, "+")) // separator for quality, wont load next line which contains quality info
        {
            skipNext = true;
            continue;
        }

        if (!startsWith(line, "@"))
        {
            sequence += line;
            continue;
        }

        // name will be empty in initial pass, so have to check every time.
        if (name.length() > 0)
        {
            std::shared_ptr<NamedSequnce> reading(new NamedSequnce(name, sequence));
            readings.push_back(reading);
            name = "";
            sequence = "";
        }

        auto rawName = line.substr(1);
        name = trim(rawName);
    }

    std::shared_ptr<NamedSequnce> reading(new NamedSequnce(name, sequence));
    readings.push_back(std::move(reading));

    file.close();
    return readings;
}