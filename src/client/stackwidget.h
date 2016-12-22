#ifndef STACKWIDGET_H
#define STACKWIDGET_H

#include <QWidget>
#include <QStackedLayout>

#include "gamelobby.h"
#include "client.h"
#include "userlistwidget.h"
#include "bagwidget.h"
#include "fightwidget.h"
#include "scratchwidget.h"

namespace Ui {
class StackWidget;
}

// 管理多个视图的切换
class StackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StackWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~StackWidget();

signals:
    // 刷新用户列表信号
    void refreshUserList();
    // 刷新用户背包信号
    void refreshBag();
    // 刷新对战界面信号
    void refreshFight();

public slots:
    // 设置当前显示的视图
    // @param:
    //      index 视图的编号
    void SetCurrentIndex(int index);

    // 返回到主界面
    void BackToLobby();


private:
    // 初始化UI界面
    void InitUi();

    // 初始化信号槽
    void InitConnect();

    // 该widget的ui界面指针
    Ui::StackWidget *ui;
    // 与服务器连接的socket指针
    Connor_Socket::Client *_client;

    // 切换视图
    QStackedLayout *_stackLayout;
    // 各个分视图
    GameLobby *_gameLobby;
    UserListWidget *_userListWidget;
    BagWidget *_bagWidget;
    FightWidget *_fightWidget;
    ScratchWidget *_scratchWidget;

};

#endif // STACKWIDGET_H
