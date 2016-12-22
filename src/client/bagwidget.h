#ifndef BAGWIDGET_H
#define BAGWIDGET_H
#include <QWidget>
#include "client.h"

namespace Ui {
class BagWidget;
}

class BagWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BagWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~BagWidget();

signals:
    // 返回到主界面信号
    void back();

public slots:
    // 设置背包界面的内容
    void SetBag();

private:
    // 初始化UI界面
    void InitUi();

    // 初始化信号槽
    void InitConnect();

    // 该widget的ui界面指针
    Ui::BagWidget *ui;
    // 与服务器连接的socket指针
    Connor_Socket::Client *_client;
};

#endif // BAGWIDGET_H
