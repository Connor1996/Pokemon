#ifndef SCRATCHWIDGET_H
#define SCRATCHWIDGET_H

#include <QWidget>
#include "client.h"

namespace Ui {
class ScratchWidget;
}

// 抓取界面
class ScratchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScratchWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~ScratchWidget();

signals:
    // 返回到主界面信号
    void back();

public slots:
    // 重载事件过滤器
    bool eventFilter(QObject *watched, QEvent *event);

private:
    // 初始化UI界面
    void InitUi();

    // 初始化信号槽
    void InitConnect();

    // 该widget的ui界面指针
    Ui::ScratchWidget *ui;
    // 与服务器连接的socket指针
    Connor_Socket::Client *_client;
};

#endif // SCRATCHWIDGET_H
