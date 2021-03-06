﻿#include "fightwidget.h"
#include "fightroom.h"
#include "ui_fightwidget.h"

#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>

#include "../pokemonfactory.h"
#include "../define.h"
#include "../model/pokemonlist.h"
#include "include/json.hpp"
using json = nlohmann::json;

FightWidget::FightWidget(Connor_Socket::Client *client, QWidget *parent) :
    _parent(parent),
    _client(client),
    _select(nullptr), // 默认为不选中任何小精灵
    ui(new Ui::FightWidget)
{
    ui->setupUi(this);
    InitUi();
    InitConnect();
}

void FightWidget::InitUi()
{
    setFixedSize(1280, 720);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/fight");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    ui->returnButton->resize(48, 48);

    // 获取对战精灵列表
    json sendInfo = {
        {"define", GET_POKEMON_LIST}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == QUERY_SUCCESS)
    {
        // 设置显示对战精灵的列表
        for(const auto& item : receiveInfo["info"])
        {
            QListWidgetItem *listItem = new QListWidgetItem(ui->listWidget);
            listItem->setText(QString::fromStdString(item.get<std::string>()));
            listItem->setTextAlignment(Qt::AlignHCenter);
            ui->listWidget->addItem(listItem);
        }
    }
    else
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获得对战精灵列表失败"));
    }


}

void FightWidget::InitConnect()
{
    // 返回按钮点击触发back信号，使得退回主界面
    connect(ui->returnButton, &QPushButton::clicked, [this](){ emit back();});
    // 升级赛
    connect(ui->fightButton, &QPushButton::clicked, [=](){
        this->FightBegin(false);
    });
    // 决斗赛
    connect(ui->fightButton_2, &QPushButton::clicked, [=](){
        this->FightBegin(true);
    });
}

bool FightWidget::eventFilter(QObject *watched, QEvent *event)
{ 
    // 只针对QLabel对象
    if (watched->inherits("QLabel"))
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            // 没有选择，则设置选中目标被选中
            if (_select == nullptr)
            {
                _select = watched;
                reinterpret_cast<QLabel*>(watched)->setStyleSheet("background: rgba(0,0,0, 20%);"
                                                              "border-radius: 5px");
            }
            // 之前有选择
            else
            {
                // 之前的选择和现在的选择一样，则取消选中
                if (watched == _select)
                {
                    _select = nullptr;
                    reinterpret_cast<QLabel*>(watched)->setStyleSheet("");
                }
                // 更新选中目标
                else
                {
                    reinterpret_cast<QLabel*>(_select)->setStyleSheet("");
                    _select = watched;
                    reinterpret_cast<QLabel*>(watched)->setStyleSheet("background: rgba(0,0,0, 20%);"
                                                                  "border-radius: 5px");
                }
            }
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void FightWidget::SetBag()
{
    // 初始化_select
    _select = nullptr;
    removeEventFilter(this);

    // 获得用户背包内容
    auto username = _client->GetUserName();
    json sendInfo = {
        {"define", GET_USER_BAG},
        {"username", username}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取背包信息失败"));
        return;
    }

    // 判断是否已经有widget，有则删除
    if(ui->bagListArea->widget() != 0)
        delete ui->bagListArea->widget();


    QWidget *containWidget = new QWidget(ui->bagListArea);
    containWidget->setStyleSheet("background-color: rgb(255, 233, 140);");

    QGridLayout *gridLayout = new QGridLayout(containWidget);
    containWidget->setLayout(gridLayout);

    // 构建显示用户背包内容的gridlayout
    auto row = 1, col = 1;
    for (const auto& item: receiveInfo["info"])
    {
        json itemInfo = json::parse(item.get<std::string>());

        // 显示精灵图片
        QLabel *picLabel = new QLabel();
        QPixmap pic(QString::fromStdString("://images/static/" +
                                            itemInfo["name"].get<std::string>() + ".png"));
        picLabel->setPixmap(pic.scaled(120, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        picLabel->installEventFilter(this);

        // 构建提示字符串
        std::string infoStr;
        for (json::iterator it = itemInfo.begin(); it != itemInfo.end(); ++it)
            infoStr += it.key() + " : " + it.value().get<std::string>() + "\n";
        infoStr.pop_back();
        picLabel->setToolTip(QString::fromStdString(infoStr));

        // 显示等级
        QLabel *textLabel = new QLabel();
        textLabel->setText(QString::fromStdString("Lv." + itemInfo["level"].get<std::string>()));
        textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");
        textLabel->setAlignment(Qt::AlignCenter);

        QVBoxLayout *rowlayout = new QVBoxLayout();
        rowlayout->addWidget(picLabel);
        rowlayout->addWidget(textLabel);

        gridLayout->addLayout(rowlayout, row, col);

        if (++col > 3)
        {
            row++;
            col = 1;
        }
    }
    // 用空layout填充第一行的空位置
    if (row == 1 && col != 1)
        for (int i = col; i <= 3; i++)
        {
            QVBoxLayout *rowlayout = new QVBoxLayout();
            QLabel *picLabel = new QLabel();
            picLabel->resize(120, 120);
            rowlayout->addWidget(picLabel);
            gridLayout->addLayout(rowlayout, 1, i);
        }

    gridLayout->setAlignment(Qt::AlignTop);
    ui->bagListArea->setWidget(containWidget);

}

void FightWidget::FightBegin(bool isLose)
{
    // 判断是否选择对战精灵
    if (_select == nullptr || ui->listWidget->currentItem() == nullptr)
    {
        QMessageBox::information(this, "ERROR", QString::fromLocal8Bit("请选择对战的小精灵"));
        return;
    }

    // 构建精灵对象
    Pokemon *fighter = PokemonFactory::CreateUser(
                reinterpret_cast<QLabel *>(_select)->toolTip().toStdString());
    Pokemon *againster = PokemonFactory::CreateComputer(
                ui->listWidget->currentItem()->text().toStdString(), _client);

    // 创建FightRoom
    FightRoom *fightRoom = new FightRoom(fighter, againster, _client, isLose);
    connect(fightRoom, SIGNAL(isClosed()), this, SLOT(show()));
    connect(fightRoom, SIGNAL(isClosed()), this, SLOT(SetBag()));

    this->hide();
    fightRoom->show();

}

FightWidget::~FightWidget()
{
    delete ui;
}
