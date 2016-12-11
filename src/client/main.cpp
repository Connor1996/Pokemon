#include "widget.h"
#include <QApplication>
#include <QTextCodec>

#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[])
{
   // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    int ret;

    try{
        QApplication a(argc, argv);
        Widget w;
        w.show();

        ret = a.exec();
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }

    return ret;

}


