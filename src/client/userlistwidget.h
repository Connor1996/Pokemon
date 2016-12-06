#ifndef USERLISTWIDGET_H
#define USERLISTWIDGET_H

#include <QWidget>
#include "client.h"

namespace Ui {
class UserListWidget;
}

class UserListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserListWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~UserListWidget();

signals:
    void back();

public slots:
    void Back();

private:
    Ui::UserListWidget *ui;

    Connor_Socket::Client *_client;

    void InitUi();
    void InitConnect();

    void SetOnlineList();
};

#endif // USERLISTWIDGET_H
