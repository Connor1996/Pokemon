#ifndef MODEL_H
#define MODEL_H

#ifdef __SERVER__
#include "server/ormlite.h"

using namespace ORMLite;
#endif


struct MyClass
{
    int id;
    double score;
    std::string name;
#ifdef __SERVER__
    ORMAP(MyClass, id, score, name);
#endif
};

#endif // MODEL_H
