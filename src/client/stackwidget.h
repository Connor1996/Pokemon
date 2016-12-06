#ifndef STACKWIDGET_H
#define STACKWIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include "gamelobby.h"
#include "client.h"
#include "userlistwidget.h"

namespace Ui {
class StackWidget;
}

class StackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StackWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~StackWidget();

public slots:
    void SetCurrentIndex(int index);
    void BackToLobby();


private:
    Ui::StackWidget *ui;
    QStackedLayout *_stackLayout;

    GameLobby *_gameLobby;
    UserListWidget *_userListWidget;

    Connor_Socket::Client *_client;
};

#endif // STACKWIDGET_H
