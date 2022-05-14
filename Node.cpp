#include "Node.h"

Node::Node(Type t, string name, int len)
{
    type = t;
    id = name;
    key = name;
    length = len;
}

Node::Node(Type t, string name, int len, Node *c, bool isCompl)
{
    string newKey = name;
    if (isCompl)
    {
        newKey = newKey + COMPLEMENT_SUFFIX;
    }

    type = t;
    id = name;
    key = newKey;
    complement = c;
    length = len;
}

void Node::addOverlap(Edge *overlap)
{
    this->overlaps.push_back(overlap);
}

bool Node::isComplement()
{
    return !(this->key == this->id);
}