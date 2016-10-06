// 引入单元测试库——catch
#ifdef __DEBUG__
#   define CATCH_CONFIG_RUNNER
#   include "catch.hpp"
#endif

#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    Widget w;
//    w.show();
#ifdef __DEBUG__
    Catch::Session().run(argc, argv);
#endif
    //return a.exec();

}
