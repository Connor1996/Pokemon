#ifndef USERLISTWIDGET_H
#define USERLISTWIDGET_H

#include <QWidget>
#include "client.h"
#include <QSignalMapper>

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
    void SetUserList();
    void ShowBag(QString username);

private:
    Ui::UserListWidget *ui;

    Connor_Socket::Client *_client;
    QSignalMapper *_signalMapper;

    void InitUi();
    void InitConnect();
};

#endif // USERLISTWIDGET_H
