#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "socket.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void InitConnect();
    void InitUi();

private slots:
    void Login();

private:
    Ui::Widget *ui;
    Socket::Client *client;
};

#endif // WIDGET_H
