// 引入单元测试库——catch
#ifdef __DEBUG__
#   define CATCH_CONFIG_RUNNER
#   include "include/catch.hpp"
#endif

#include <iostream>
#include "../socket.h"
#include "../widget.h"
#include <QApplication>
#include <thread>

int main(int argc, char *argv[])
{
#ifdef __DEBUG__
    Catch::Session().run(argc, argv);
#endif

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}


