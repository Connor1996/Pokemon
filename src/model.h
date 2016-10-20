#ifndef MODEL_H
#define MODEL_H

#include "ormlite.h"

using namespace ORMLite;

struct MyClass
{
    int id;
    double score;
    std::string name;

    ORMAP(MyClass, id, score, name);
};

#endif // MODEL_H
