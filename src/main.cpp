// 引入单元测试库——catch
#ifdef __DEBUG__
#   define CATCH_CONFIG_RUNNER
#   include "include/catch.hpp"
#endif

#include "widget.h"
#include <QApplication>
#include "ormlite.h"

using namespace ORMLite;

struct MyClass
{
    int id;
    double score;
    std::string name;

    ORMAP(MyClass, id, score, name);
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
#ifdef __DEBUG__
    Catch::Session().run(argc, argv);
#endif
    return a.exec();

}
