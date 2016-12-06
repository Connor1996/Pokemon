#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <QWidget>
#include <QStackedLayout>s
#include "client.h"

namespace Ui {
class GameLobby;
}

class GameLobby : public QWidget
{
    Q_OBJECT

public:
    explicit GameLobby(Connor_Socket::Client *client, QWidget *parent = 0);
    ~GameLobby();

signals:
    void clicked(int type);
    void closeAll();

private slots:
    void LogOut();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    void InitConnect();
    void InitUi();

    Ui::GameLobby *ui;

    Connor_Socket::Client *_client;

    QStackedLayout *_stackLayout;
};

#endif // GAMELOBBY_H
