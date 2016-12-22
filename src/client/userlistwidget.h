#ifndef USERLISTWIDGET_H
#define USERLISTWIDGET_H

#include <QWidget>
#include "client.h"
#include <QSignalMapper>

namespace Ui {
class UserListWidget;
}

// 用户列表界面
class UserListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserListWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~UserListWidget();

signals:
    // 返回到主界面信号
    void back();

public slots:
    // 设置用户列表界面的内容
    void SetUserList();

    // 显示背包内容
    // @param:
    //      username 要查看背包的所属用户
    void ShowBag(QString username);

private:
    // 初始化UI界面
    void InitUi();

    // 初始化信号槽
    void InitConnect();

    // 该widget的ui界面指针
    Ui::UserListWidget *ui;
    // 与服务器连接的socket指针
    Connor_Socket::Client *_client;
    // 在选择用户背包按钮的mapper
    QSignalMapper *_signalMapper;

};

#endif // USERLISTWIDGET_H
