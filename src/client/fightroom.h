#ifndef FIGHTROOM_H
#define FIGHTROOM_H

#include <QWidget>
#include <QSignalMapper>
#include <QLabel>

#include "../pokemon.h"
#include "client.h"


using Client = Connor_Socket::Client;

namespace Ui {
class FightRoom;
}

class FightRoom : public QWidget
{
    Q_OBJECT

public:
    explicit FightRoom(Pokemon *fighter, Pokemon *againster, Client *client,
                       bool isLose, QWidget *parent = 0);
    ~FightRoom();
    void Fight();
    void FightRoom::closeEvent(QCloseEvent *event);

signals:
    void attack(QLabel *, QLabel *);
    void attack_SP(std::pair<Pokemon *, QLabel *> *, std::pair<Pokemon *, QLabel *> *);
    void over(Pokemon *);
    void hurt(QLabel *attacker, QLabel *suffer);
    void isClosed();
    void clearText();
    void hideLabel();

private slots:
    void setAnimation(QLabel *, QLabel *);
    void setAnimation_SP(std::pair<Pokemon *, QLabel *> *, std::pair<Pokemon *, QLabel *> *);
    void GameComplete(Pokemon *);
    void UpdateHp(QLabel *attacker, QLabel *suffer);
    void setText();
    void setLabel();
    void Choose(int);

private:
    void InitUi();
    void InitConnect();

    Ui::FightRoom *ui;

    std::pair<Pokemon *, QLabel *> _fighter;
    std::pair<Pokemon *, QLabel *> _againster;

    Connor_Socket::Client *_client;
    bool _isLose;
    QSignalMapper *_signalMapper;

    bool _quit;
};

#endif // FIGHTROOM_H
