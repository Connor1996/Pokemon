#ifndef STACKWIDGET_H
#define STACKWIDGET_H

#include <QWidget>
#include <QStackedLayout>

#include "gamelobby.h"
#include "client.h"
#include "userlistwidget.h"
#include "bagwidget.h"
#include "fightwidget.h"

namespace Ui {
class StackWidget;
}

class StackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StackWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~StackWidget();

signals:
    void refreshUserList();
    void refreshBag();

public slots:
    void SetCurrentIndex(int index);
    void BackToLobby();


private:
    void InitUi();
    void InitConnect();

    Ui::StackWidget *ui;
    QStackedLayout *_stackLayout;

    GameLobby *_gameLobby;
    UserListWidget *_userListWidget;
    BagWidget *_bagWidget;
    FightWidget *_fightWidget;

    Connor_Socket::Client *_client;
};

#endif // STACKWIDGET_H
