#ifndef FIGHTROOM_H
#define FIGHTROOM_H

#include <QWidget>
#include "../pokemon.h"

namespace Ui {
class FightRoom;
}

class FightRoom : public QWidget
{
    Q_OBJECT

public:
    explicit FightRoom(Pokemon *fighter, Pokemon *againster, QWidget *parent = 0);
    ~FightRoom();

private:
    void Fight();

    Ui::FightRoom *ui;

    Pokemon *_fighter;
    Pokemon *_againster;
};

#endif // FIGHTROOM_H
