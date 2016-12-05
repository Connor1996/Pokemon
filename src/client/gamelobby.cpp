#include "gamelobby.h"
#include "ui_gamelobby.h"
#include <QMovie>
#include <QPropertyAnimation>

#include "include/json.hpp"
#include "../define.h"

using json = nlohmann::json;

GameLobby::GameLobby(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent), _client(client),
    ui(new Ui::GameLobby)
{
    ui->setupUi(this);
    InitConnect();

    QMovie *pikachu = new QMovie(":/pikachu");
    ui->label->setMovie(pikachu);
    pikachu->start();
    QPropertyAnimation animation(ui->label, "geometry");
    animation.setDuration(1000);
    animation.setStartValue(QRect(0, 0, 0, 0));
    animation.setEndValue(QRect(250, 250, 100, 30));
    animation.start();
}

GameLobby::~GameLobby()
{
    delete ui;
}

void GameLobby::InitConnect()
{
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(GetOnlineList()));
}

void GameLobby::GetOnlineList()
{
    json sendInfo = {
        {"type", GET_ONLINE_LIST}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));
    std::cout << receiveInfo << std::endl;
}
