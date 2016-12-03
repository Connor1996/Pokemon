#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <QWidget>
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

private slots:
    void GetOnlineList();

private:
    void InitConnect();

    Ui::GameLobby *ui;

    Connor_Socket::Client *_client;
};

#endif // GAMELOBBY_H
