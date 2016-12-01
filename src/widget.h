#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "client/client.h"

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
    // 登陆
    void Login();

    // 注册
    void Signup();

private:
    Ui::Widget *ui;
    Connor_Socket::Client *_client;
};

#endif // WIDGET_H
