#include "fightroom.h"
#include "ui_fightroom.h"

#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QCloseEvent>

#include <iostream>
#include <thread>

#include "include/json.hpp"
#include "../define.h"
using json = nlohmann::json;

FightRoom::FightRoom(Pokemon *fighter, Pokemon *againster, Client *client,
                     QWidget *parent) :
    QWidget(parent), _fighter(std::make_pair(fighter, new QLabel(this))),
    _againster(std::make_pair(againster, new QLabel(this))), _client(client),
    ui(new Ui::FightRoom), _quit(false)
{
    ui->setupUi(this);
    InitUi();
    InitConnect();


    std::thread th(std::bind(&FightRoom::Fight, this));
    th.detach();
}

void FightRoom::InitUi()
{
    setFixedSize(1280, 720);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/fightroom");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    //ui->returnButton->resize(48, 48);

    QLabel *op1 = _fighter.second;
    // 设置两个小精灵动图
    op1->setMovie(new QMovie(QString::fromStdString("://images/dynamic/"
                                                    + _fighter.first->GetName() + "_left.gif")));
    op1->movie()->start();
    op1->setGeometry(238, 359, 192, 192);

    QLabel *op2 = _againster.second;
    op2->setMovie(new QMovie(QString::fromStdString("://images/dynamic/"
                                                    + _againster.first->GetName() + ".gif")));
    op2->movie()->start();
    op2->setGeometry(860, 359, 192, 192);

    ui->fighterLabel->setText(QString::fromStdString(_fighter.first->GetName()));
    ui->fighterBar->setRange(0, _fighter.first->GetHealthPoint());
    ui->fighterBar->setValue(_fighter.first->GetHealthPoint());

    ui->againsterLabel->setText(QString::fromStdString(_againster.first->GetName()));
    ui->againsterBar->setRange(0, _againster.first->GetHealthPoint());
    ui->againsterBar->setValue(_againster.first->GetHealthPoint());
}

void FightRoom::InitConnect()
{
    connect(this, SIGNAL(attack(QLabel *, QLabel *)), this, SLOT(setAnimation(QLabel *, QLabel *)));
    connect(this, SIGNAL(over(QString)), this, SLOT(GameComplete(QString)));
    connect(this, SIGNAL(hurt(QLabel *, QLabel *)), this, SLOT(UpdateHp(QLabel *, QLabel *)));
    connect(this, SIGNAL(clearText()), this, SLOT(setText()));
}

void FightRoom::closeEvent(QCloseEvent *event)
{
    emit isClosed();
    _quit = true;
    event->accept();
}

void FightRoom::setAnimation(QLabel *attacker, QLabel *suffer)
{
    auto x = attacker->x();
    auto y = attacker->y();

    QSequentialAnimationGroup *actions = new QSequentialAnimationGroup(this);

    // 攻击方出击
    QPropertyAnimation *animation1 = new QPropertyAnimation(attacker, "pos");
    animation1->setDuration(1000);
    animation1->setEasingCurve(QEasingCurve::InQuad);
    animation1->setEndValue(QPoint(suffer->x() - 10, suffer->y()));
    actions->addAnimation(animation1);

    // 攻击方回退
    QPropertyAnimation *animation2 = new QPropertyAnimation(attacker, "pos");
    animation2->setDuration(500);
    animation2->setEndValue(QPoint(x, y));
    actions->addAnimation(animation2);

    actions->start();
}

void FightRoom::setText()
{
    ui->label->setText("");
    ui->label->setGeometry(340, 440, 81, 31);
    ui->label_2->setText("");
    ui->label_2->setGeometry(890, 440, 81, 31);
}

void FightRoom::GameComplete(QString winner)
{
    QMessageBox::information(this, "INFO", "winner is " + winner);
    if (winner.toStdString() == _fighter.first->GetName())
    {
        json sendInfo = {
            {"type", GAME_WIN},
            {"name", _againster.first->GetName()}
        };
        json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    }
    else
    {
        json sendInfo = {
            {"type", GAME_LOSE},
            {"name", _againster.first->GetName()}
        };
        json receiveInfo = json::parse(_client->Send(sendInfo.dump()));
    }
    this->close();
}

void FightRoom::UpdateHp(QLabel *attacker, QLabel *suffer)
{
    QParallelAnimationGroup *actions = new QParallelAnimationGroup;
    QPropertyAnimation *animation1;

    if (suffer == _fighter.second)
    {
        animation1 = new QPropertyAnimation(ui->label, "pos");
        int num = ui->fighterBar->value() - (int)_fighter.first->GetHp();
        //std::cout << num << std::endl;
        if (num == 0)
            ui->label->setText(QString::fromLocal8Bit("闪避"));
        else
            ui->label->setText(QString::fromStdString("-" + std::to_string(num)));
        animation1->setEndValue(QPoint(ui->label->x(), ui->label->y() - 10));
    }
    else
    {
        animation1 = new QPropertyAnimation(ui->label_2, "pos");
        int num = ui->againsterBar->value() - (int)_againster.first->GetHp();
        //std::cout << num << std::endl;
        if (num == 0)
            ui->label_2->setText(QString::fromLocal8Bit("闪避"));
        else
            ui->label_2->setText(QString::fromStdString("-" + std::to_string(num)));
        animation1->setEndValue(QPoint(ui->label_2->x(), ui->label_2->y() - 10));
    }

    animation1->setDuration(400);
    actions->addAnimation(animation1);

    // 被攻击方震动
    auto dx = suffer->x();
    auto dy = suffer->y();
    QPropertyAnimation *animation3 = new QPropertyAnimation(suffer, "pos");
    animation3->setDuration(200);
    animation3->setLoopCount(2);
    animation3->setKeyValueAt(0, QPoint(dx - 3, dy - 3));
    animation3->setKeyValueAt(0.1, QPoint(dx + 6, dy + 6));
    animation3->setKeyValueAt(0.2, QPoint(dx - 6, dy + 6));
    animation3->setKeyValueAt(0.3, QPoint(dx + 6, dy - 6));
    animation3->setKeyValueAt(0.4, QPoint(dx - 6, dy - 6));
    animation3->setKeyValueAt(0.5, QPoint(dx + 6, dy + 6));
    animation3->setKeyValueAt(0.6, QPoint(dx - 6, dy + 6));
    animation3->setKeyValueAt(0.7, QPoint(dx + 6, dy - 6));
    animation3->setKeyValueAt(0.8, QPoint(dx - 6, dy - 6));
    animation3->setKeyValueAt(0.9, QPoint(dx + 6, dy + 6));
    animation3->setKeyValueAt(1, QPoint(dx, dy));
    actions->addAnimation(animation3);

    actions->start(QAbstractAnimation::DeleteWhenStopped);

    if (attacker == _fighter.second)
    {
        ui->againsterBar->setValue(_againster.first->GetHp());
        //ui->againsterBar->update();
        ui->againsterBar->repaint();
    }
    else
    {
        ui->fighterBar->setValue(_fighter.first->GetHp());
        //ui->fighterBar->update();
        ui->fighterBar->repaint();
    }
}

// op1为玩家，op2为电脑
void FightRoom::Fight()
{
    std::string winner;

    auto speed1 = _fighter.first->GetAttackFrequence();
    auto speed2 = _againster.first->GetAttackFrequence();

    // 时间轴
    size_t time1 = 0;
    size_t time2 = 0;

    while (++time1, ++time2)
    {
       if (_quit)
           return;

       if (time1 == speed1)
       {
           // op1进行攻击
           auto damage = _fighter.first->Attack(_againster.first);
           auto isDead = _againster.first->Hurt(damage);

           emit attack(_fighter.second, _againster.second);
           std::this_thread::sleep_for(std::chrono::seconds(1));
           emit hurt(_fighter.second, _againster.second);
           std::this_thread::sleep_for(std::chrono::seconds(1));
           emit clearText();

           if (isDead)
           {
               winner = _fighter.first->GetName();
               break;
           }
           time1 = 0;
       }

       if (time2 == speed2)
       {

           auto damage = _againster.first->Attack(_fighter.first);
           //std::cout << "damage:" << std::to_string(damage) << std::endl;
           auto isDead = _fighter.first->Hurt(damage);

           emit attack(_againster.second, _fighter.second);
           std::this_thread::sleep_for(std::chrono::seconds(1));
           emit hurt(_againster.second, _fighter.second);
           std::this_thread::sleep_for(std::chrono::seconds(1));
           emit clearText();

           if (isDead)
           {
               winner = _againster.first->GetName();
               break;
           }
           time2 = 0;
       }
    }

    emit over(QString::fromStdString(winner));
}

FightRoom::~FightRoom()
{
    delete ui;
}
