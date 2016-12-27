#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <QString>

#include <iostream>
#include <stdexcept>

// 重载QApplication捕获异常
class MyApplication : public QApplication
{
public:
    MyApplication(int &argc, char ** argv)
        : QApplication(argc, argv)
    { }

private:
    bool notify(QObject* receiver, QEvent* event)
    {
        try
        {
            return QApplication::notify(receiver, event);
        }
        catch (std::exception& e)
        {
            QMessageBox::information(NULL, "ERROR", e.what());
            return false;
        }
    }
};


int main(int argc, char *argv[])
{
    MyApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}


