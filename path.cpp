#include "path.h"
#include "Global.h"

bool Path::isDeadEnd(std::unordered_map<string, Node *> lookup)
{
    return this->getEnd(lookup)->type != Type::CONTIG;
}

Edge *Path::pop()
{
    Edge *removed = this->path.back();
    this->path.pop_back();
    return removed;
}
void Path::push(Edge *edge)
{
    this->path.push_back(edge);
}

Node *Path::getStart(std::unordered_map<string, Node *> lookup)
{
    return lookup[this->path.front()->querySequenceName];
}

Node *Path::getEnd(std::unordered_map<string, Node *> lookup)
{
    return lookup[this->path.back()->targetSequenceName];
}

int Path::size()
{
    return this->path.size();
}

std::vector<Edge *> Path::getEdges()
{
    return std::vector<Edge *>(this->path);
}