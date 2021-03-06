﻿#include "fightroom.h"
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
                     bool isLose, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FightRoom),
    _client(client),
    _fighter(std::make_pair(fighter, new QLabel(this))),
    _againster(std::make_pair(againster, new QLabel(this))),
    _isLose(isLose),
    _quit(false), // 是否退出默认为false
    _signalMapper(new QSignalMapper)
{
    ui->setupUi(this);
    InitUi();
    InitConnect();

    // 启动新线程控制对战流程
    std::thread th(std::bind(&FightRoom::Fight, this));
    th.detach();
}

void FightRoom::InitUi()
{
    setFixedSize(1280, 720);
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/fightroom");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    ui->chooseWidget->hide();

    // 设置两个小精灵贴图
    QLabel *op1 = _fighter.second;
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
    // 触发普通攻击动画
    connect(this, SIGNAL(attack(QLabel *, QLabel *)), this, SLOT(setAnimation(QLabel *, QLabel *)));

    // 触发特殊攻击动画
    connect(this, SIGNAL(attack_SP(std::pair<Pokemon *, QLabel *> *, std::pair<Pokemon *, QLabel *> *)),
            this, SLOT(setAnimation_SP(std::pair<Pokemon *, QLabel *> *, std::pair<Pokemon *, QLabel *> *)));

    connect(this, SIGNAL(hideLabel()), this, SLOT(setLabel()));
    connect(this, SIGNAL(over(Pokemon *)), this, SLOT(GameComplete(Pokemon *)));
    connect(this, SIGNAL(hurt(QLabel *, QLabel *)), this, SLOT(UpdateHp(QLabel *, QLabel *)));
    connect(this, SIGNAL(clearText()), this, SLOT(setText()));
    connect(_signalMapper, SIGNAL(mapped(int)), this, SLOT(Choose(int)));
}

void FightRoom::closeEvent(QCloseEvent *event)
{
    // 关闭时间时将_quit变量设置为true，以终止对战控制线程
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

void FightRoom::setAnimation_SP(std::pair<Pokemon *, QLabel *> *attacker,
                                std::pair<Pokemon *, QLabel *> *suffer)
{
    // 攻击方火球出击
    auto setAnimation = [&](QLabel *label){
        QPropertyAnimation *animation = new QPropertyAnimation(label, "pos");
        animation->setDuration(1000);
        animation->setEndValue(QPoint(suffer->second->x() + 40, suffer->second->y() + 110));
        animation->start();
    };

    // 攻击方为用户
    if (attacker->second == _fighter.second)
    {
        std::cout << GET_CLASS_TYPE(*attacker->first) << std::endl;
        ui->fireLeft->setStyleSheet("border-image: url(://images/" +
                                    QString::fromStdString(GET_CLASS_TYPE(*attacker->first))
                                    + "_left.png)");
        ui->fireLeft->setGeometry(attacker->second->x() + 40,
                                  attacker->second->y() + 110, 60, 30);
        setAnimation(ui->fireLeft);
    }
    // 攻击方为电脑
    else
    {
        ui->fireRight->setStyleSheet("border-image: url(://images/" +
                                      QString::fromStdString(GET_CLASS_TYPE(*attacker->first))
                                      + ".png)");
        ui->fireRight->setGeometry(attacker->second->x() + 40,
                                   attacker->second->y() + 110, 60, 30);
        setAnimation(ui->fireRight);
    }

}

void FightRoom::setText()
{
    ui->label->setText("");
    ui->label->setStyleSheet("font: 75 14pt \"Arial Black\";"
                             "color: rgb(255, 23, 26);");
    ui->label->setGeometry(340, 400, 120, 120);
    ui->label_2->setText("");
    ui->label_2->setStyleSheet("font: 75 14pt \"Arial Black\";"
                             "color: rgb(255, 23, 26);");
    ui->label_2->setGeometry(850, 400, 120, 120);
}

void FightRoom::setLabel()
{
    ui->fireLeft->setStyleSheet("");
    ui->fireRight->setStyleSheet("");
}

void FightRoom::GameComplete(Pokemon* winner)
{
    // 挑战成功
    if (winner == _fighter.first)
    {
        auto isUpgrade = _fighter.first->Upgrade(_againster.first->GetExp());

        // 将更新信息后的用户小精灵发送回服务器
        json sendInfo = {
            {"define", GAME_WIN},
            {"get", _againster.first->GetName()},
            {"name", _fighter.first->GetName()},
            {"type", (int)_fighter.first->GetType()},
            {"attackPoint", _fighter.first->GetAttackPoint()},
            {"defensePoint", _fighter.first->GetDefensePoint()},
            {"healthPoint", _fighter.first->GetHealthPoint()},
            {"attackFrequence", _fighter.first->GetAttackFrequence()},
            {"property", GET_CLASS_TYPE(*_fighter.first)},
            {"level", _fighter.first->GetLevel()},
            {"exp", _fighter.first->GetExp()},
            {"id", _fighter.first->GetId()}
        };
        json receiveInfo = json::parse(_client->Send(sendInfo.dump()));
        if (receiveInfo["define"].get<int>() == ACCEPT)
        {
            std::string info = "挑战成功!\n获得目标小精灵：" + _againster.first->GetName() + "\n" +
                           "获得经验：" + std::to_string(_againster.first->GetExp());
            if (isUpgrade)
                info += "\n小精灵升级到" + std::to_string(_fighter.first->GetLevel()) + "级";
            QMessageBox::information(this, "INFO", QString::fromLocal8Bit(info.c_str()));
        }
        else
            QMessageBox::information(this, "ERROR", QString::fromLocal8Bit("服务器出错"));
        this->close();
    }
    // 挑战失败
    else
    {
        json sendInfo = {
            {"define", GAME_LOSE},
            {"isLose", _isLose}
        };
        json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

        // 升级赛的情况
        if (_isLose == false)
        {
            QMessageBox::information(this, "INFO", QString::fromLocal8Bit("挑战失败"));
            this->close();
            return;
        }

        // 构建三个丢弃小精灵的显示窗口
        int count = 0;
        for (const auto& item: receiveInfo["info"])
        {
            count++;
            json itemInfo = json::parse(item.get<std::string>());

            // 显示精灵图片
            QPushButton *picLabel = new QPushButton();
            picLabel->setFlat(true);
            picLabel->setMinimumSize(120, 120);
            picLabel->setMaximumSize(120, 120);
            picLabel->setStyleSheet("border-image: url(://images/static/" +
                                    QString::fromStdString(itemInfo["name"].get<std::string>()) + ".png);");
            _signalMapper->setMapping(picLabel, std::stoi(itemInfo["id"].get<std::string>()));
            connect(picLabel, SIGNAL(clicked()), _signalMapper, SLOT(map()));

            // 构建提示字符串
            std::string infoStr;
            QLabel *textLabel = new QLabel();
            for (json::iterator it = itemInfo.begin(); it != itemInfo.end(); ++it)
                infoStr += it.key() + " : " + it.value().get<std::string>() + "\n";
            infoStr.pop_back();
            textLabel->setStyleSheet("font: 75 8pt; color: rgb(255, 255, 255);");
            textLabel->setText(QString::fromStdString(infoStr));


            QVBoxLayout *itemLayout = new QVBoxLayout();
            itemLayout->addWidget(picLabel);
            itemLayout->addWidget(textLabel);

            ui->chooseLayout->addLayout(itemLayout);
        }
        ui->chooseLayout->setAlignment(Qt::AlignHCenter);
        ui->chooseWidget->show();
    }
}

void FightRoom::Choose(int id)
{
    // 将用户要丢弃的小精灵id发送回服务器
    json sendInfo = {
        {"define", LOSE_POKEMON},
        {"id", id}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));
    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
        QMessageBox::information(this, "ERROR", QString::fromLocal8Bit("服务器出错"));
    this->close();
}


void FightRoom::UpdateHp(QLabel *attacker, QLabel *suffer)
{
    QParallelAnimationGroup *actions = new QParallelAnimationGroup;
    QPropertyAnimation *animation1;

    // 被攻击方为用户
    if (suffer == _fighter.second)
    {
        animation1 = new QPropertyAnimation(ui->label, "pos");
        int num = ui->fighterBar->value() - (int)_fighter.first->GetHp();         
        if (num == 0)
            ui->label->setText(QString::fromLocal8Bit("MISS"));
        else
        {
            if (_againster.first->IsCritical())
                ui->label->setStyleSheet("border-image: url(:/boom); font: 75 14pt \"Arial Black\";"
                                         "color: rgb(255, 23, 26);");
            ui->label->setText(QString::fromStdString("-" + std::to_string(num)));
        }
        animation1->setEndValue(QPoint(ui->label->x(), ui->label->y() - 10));
    }
    // 被攻击方为电脑
    else
    {
        animation1 = new QPropertyAnimation(ui->label_2, "pos");
        int num = ui->againsterBar->value() - (int)_againster.first->GetHp();
        if (num == 0)
            ui->label_2->setText(QString::fromLocal8Bit("MISS"));
        else
        {
            if (_fighter.first->IsCritical())
                ui->label_2->setStyleSheet("border-image: url(:/boom); font: 75 14pt \"Arial Black\";"
                                         "color: rgb(255, 23, 26);");
            ui->label_2->setText(QString::fromStdString("-" + std::to_string(num)));
        }
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
        ui->againsterBar->setValue(_againster.first->GetHp());
    else
        ui->fighterBar->setValue(_fighter.first->GetHp());

}

// op1为玩家，op2为电脑
void FightRoom::Fight()
{
    Pokemon *winner = nullptr;

    auto speed1 = _fighter.first->GetAttackFrequence();
    auto speed2 = _againster.first->GetAttackFrequence();

    // 时间轴
    size_t time1 = 0;
    size_t time2 = 0;

    // 一方攻击另外一方
    auto fight = [&](std::pair<Pokemon *, QLabel *> *op1,
            std::pair<Pokemon *, QLabel *> *op2)
    {
        auto damage = op1->first->Attack(op2->first);
        auto isDead = op2->first->Hurt(damage);

        // 攻击动画
        if (op1->first->IsCritical())
            emit attack_SP(op1, op2);
        else
            emit attack(op1->second, op2->second);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // 是否为暴击
        if (op1->first->IsCritical())
            emit hideLabel();
        // 受伤动画
        emit hurt(op1->second, op2->second);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        emit clearText();

        // 判断对手是否死亡
        if (isDead)
            winner = op1->first;

        return isDead;
    };

    while (++time1, ++time2)
    {
       if (_quit)
           return;

       if (time1 == speed1)
       {
           // op1进行攻击
           if (fight(&_fighter, &_againster))
               break;

           time1 = 0;
       }

       if (time2 == speed2)
       {
           // op2进行攻击
           if (fight(&_againster, &_fighter))
               break;

           time2 = 0;
       }
    }

    emit over(winner);
}

FightRoom::~FightRoom()
{
    delete ui;
}
