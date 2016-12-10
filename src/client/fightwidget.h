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

private:
    void InitUi();
    void InitConnect();

    Ui::FightWidget *ui;
    Connor_Socket::Client *_client;
};

#endif // FIGHTWIDGET_H
