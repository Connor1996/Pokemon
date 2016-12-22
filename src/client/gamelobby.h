#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <QWidget>
#include <QStackedLayout>
#include "client.h"

namespace Ui {
class GameLobby;
}

// 主界面
class GameLobby : public QWidget
{
    Q_OBJECT

public:
    explicit GameLobby(Connor_Socket::Client *client, QWidget *parent = 0);
    ~GameLobby();

signals:
    // 鼠标点击在相应的区域信号
    void clicked(int type);
    // 关闭程序信号
    void closeAll();

private slots:
    // 登出
    void LogOut();

protected:
    // 重载鼠标点击事件
    void mousePressEvent(QMouseEvent *event);

private:
    // 初始化UI界面
    void InitUi();

    // 初始化信号槽
    void InitConnect();

    // 该widget的ui界面指针
    Ui::GameLobby *ui;
    // 与服务器连接的socket指针
    Connor_Socket::Client *_client;
    // 切换视图的stacklayout
    QStackedLayout *_stackLayout;
};

#endif // GAMELOBBY_H
