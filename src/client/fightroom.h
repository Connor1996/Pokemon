#ifndef FIGHTROOM_H
#define FIGHTROOM_H

#include <QWidget>
#include "../pokemon.h"

#include <QLabel>

namespace Ui {
class FightRoom;
}

class FightRoom : public QWidget
{
    Q_OBJECT

public:
    explicit FightRoom(Pokemon *fighter, Pokemon *againster, QWidget *parent = 0);
    ~FightRoom();
    void Fight();

signals:
    void attack(QLabel *, QLabel *);
    void over(QString);
    void hurt(QLabel *attacker, QLabel *suffer);

private slots:
    void setAnimation(QLabel *, QLabel *);
    void GameComplete(QString);
    void UpdateHp(QLabel *attacker, QLabel *suffer);

private:
    void InitUi();
    void InitConnect();

    Ui::FightRoom *ui;

    std::pair<Pokemon *, QLabel *> _fighter;
    std::pair<Pokemon *, QLabel *> _againster;

};

#endif // FIGHTROOM_H
