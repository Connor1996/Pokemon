﻿#include "fightwidget.h"
#include "ui_fightwidget.h"

#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>

#include "../define.h"
#include "include/json.hpp"
using json = nlohmann::json;

FightWidget::FightWidget(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent), _client(client),
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
}

void FightWidget::InitConnect()
{
    connect(ui->returnButton, SIGNAL(clicked()), this, SLOT(Back()));
}

bool FightWidget::eventFilter(QObject *watched, QEvent *event)
{
    static QObject *select = nullptr;
    if (watched->inherits("QLabel"))
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            if (select == watched)
                reinterpret_cast<QLabel*>(watched)->setStyleSheet("");
            else
                reinterpret_cast<QLabel*>(watched)->setStyleSheet("background: rgba(0,0,0, 20%);"
                                                              "border-radius: 5px");
            return true;
        }
        else
            return false;
    }
    else
        return eventFilter(watched, event);
}

void FightWidget::SetBag()
{
    auto username = _client->GetUserName();
    json sendInfo = {
        {"type", GET_USER_BAG},
        {"username", username}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    if (receiveInfo["type"].get<int>() == SERVER_ERROR)
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
    gridLayout->setAlignment(Qt::AlignTop);
    ui->bagListArea->setWidget(containWidget);

}

void FightWidget::Back()
{
    emit back();
}

FightWidget::~FightWidget()
{
    delete ui;
}
