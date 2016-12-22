#ifndef FIGHTWIDGET_H
#define FIGHTWIDGET_H

#include <QWidget>
#include "client.h"

namespace Ui {
class FightWidget;
}

// 对战选择界面
class FightWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FightWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~FightWidget();

signals:
    // 返回到主界面信号
    void back();

public slots:
    // 设置背包界面的内容
    void SetBag();

    // 对战开始
    // @param:
    //      isLose 对战失败用户是否会损失小精灵
    void FightBegin(bool isLose);

    // 重载事件过滤器
    bool eventFilter(QObject *watched, QEvent *event);

private:
    // 初始化UI界面
    void InitUi();

    // 初始化信号槽
    void InitConnect();

    // 该widget的ui界面指针
    Ui::FightWidget *ui;
    // 与服务器连接的socket指针
    Connor_Socket::Client *_client;
    // 父对象指针
    QWidget *_parent;
    // 选中出战的小精灵
    QObject *_select = nullptr;
};

#endif // FIGHTWIDGET_H
