#include "fightroom.h"
#include "ui_fightroom.h"

#include <QLabel>
#include <QMovie>
#include <QPushButton>

#include <iostream>

FightRoom::FightRoom(Pokemon *fighter, Pokemon *againster, QWidget *parent) :
    QWidget(parent), _fighter(fighter), _againster(againster),
    ui(new Ui::FightRoom)
{
    ui->setupUi(this);

    setFixedSize(1280, 720);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/fightroom");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    //ui->returnButton->resize(48, 48);

    // 设置两个小精灵动图
    QLabel *op1 = new QLabel(this);
    op1->setMovie(new QMovie(QString::fromStdString("://images/dynamic/" + _fighter->GetName() + ".gif")));
    op1->movie()->start();
    op1->setGeometry(238, 331, 192, 192);

    QLabel *op2 = new QLabel(this);
    op2->setMovie(new QMovie(QString::fromStdString("://images/dynamic/" + _againster->GetName() + ".gif")));
    op2->movie()->start();
    op2->setGeometry(913, 331, 192, 192);
}


void FightRoom::Fight()
{
    auto speed_p = _fighter->GetAttackFrequence();
    auto speed_c = _againster->GetAttackFrequence();
    std::cout << "[INFO] the speed of " << _fighter->GetName() << " is " << speed_p << std::endl;
    std::cout << "[INFO] the speed of " << _againster->GetName() << " is " << speed_c << std::endl;
    std::cout << "[INFO] Fight begin" << std::endl;
    // 时间轴
    size_t time_p = 0;
    size_t time_c = 0;

    while (time_p++, time_c++)
    {
       if (time_p == speed_p)
       {
           std::cout << "[INFO] " << _fighter->GetName() <<
                        "attack" << _againster->GetName() << std::endl;
           auto damage = _fighter->Attack(_againster);
           std::cout << "[INFO] " << _againster->GetName() << " get hurt by "
                     << damage << std::endl;
           if (_againster->Hurt(damage))
           {
               std::cout << "[INFO] " << _againster->GetName() << " is died" << std::endl;
               break;
           }
           time_p = 0;
       }

       if (time_c == speed_c)
       {
           std::cout << "[INFO] " << _againster->GetName() <<
                        "attack" << _fighter->GetName() << std::endl;
           auto damage = _againster->Attack(_fighter);
           std::cout << "[INFO] " << _fighter->GetName() << " get hurt by "
                     << damage << std::endl;
           if (_fighter->Hurt(damage))
           {
               std::cout << "[INFO] " << _fighter->GetName() << " is died" << std::endl;
               break;
           }
           time_c = 0;
       }
    }
    std:: cout << "[INFO] Fight over" << std::endl;

}

FightRoom::~FightRoom()
{
    delete ui;
}
