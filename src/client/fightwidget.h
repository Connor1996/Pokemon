#ifndef FIGHTWIDGET_H
#define FIGHTWIDGET_H

#include <QWidget>
#include "client.h"

namespace Ui {
class FightWidget;
}

class FightWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FightWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~FightWidget();

signals:
    void back();

public slots:
    void Back();
    void SetBag();
    void FightBegin(bool isLose);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void InitUi();
    void InitConnect();

    Ui::FightWidget *ui;
    QWidget *_parent;
    Connor_Socket::Client *_client;

    // 选中出战的小精灵
    QObject *_select = nullptr;
};

#endif // FIGHTWIDGET_H
