#include "userlistwidget.h"
#include "ui_userlistwidget.h"

#include <QLabel>
#include <QMovie>
#include <QMessageBox>

#include "include/json.hpp"
#include "../define.h"
using json = nlohmann::json;

UserListWidget::UserListWidget(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent), _client(client), _signalMapper(nullptr),
    ui(new Ui::UserListWidget)
{
    ui->setupUi(this);

    InitUi();
    InitConnect();

}

UserListWidget::~UserListWidget()
{
    delete ui;
}

void UserListWidget::InitUi()
{
    setFixedSize(1280, 720);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/userlist");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    ui->returnButton->resize(48, 48);
}

void UserListWidget::InitConnect()
{
    connect(ui->returnButton, SIGNAL(clicked()), this, SLOT(Back()));
}

void UserListWidget::SetUserList()
{
    // 获得在线用户
    json sendInfo = {
        {"type", GET_ONLINE_LIST}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    if (receiveInfo["type"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取用户列表失败"));
        return;
    }

    // 判断是否已经有widget，有则删除
    if(ui->userListArea->widget() != 0)
        delete ui->userListArea->widget();


    QWidget *containWidget = new QWidget(ui->userListArea);
    containWidget->setStyleSheet("background-color: rgb(255, 175, 77);");

    QVBoxLayout *listLayout = new QVBoxLayout(containWidget);
    listLayout->setAlignment(Qt::AlignTop);
    //containWidget->setLayout(listLayout);

    _signalMapper = new QSignalMapper(containWidget);


    for(const auto& item : receiveInfo["info"])
    {
        std::string str = item.get<std::string>();

        // 显示用户状态
        QLabel *stateLabel = new QLabel();
        stateLabel->setPixmap(QPixmap(":/online")
                              .scaled(36, 36, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        // 显示用户名
        QLabel *textLabel = new QLabel();
        textLabel->setText(QString::fromStdString(str));
        textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");

        QPushButton *bagButton = new QPushButton();
//        bagButton->setFlat(true);
//        bagButton->resize(36, 36);
//        bagButton->setStyleSheet("image: url(:/ball)");
        _signalMapper->setMapping(bagButton, QString::fromStdString(str));
        connect(bagButton, SIGNAL(clicked()), _signalMapper, SLOT(map()));


        // 构建显示条目的layout
        QHBoxLayout *rowlayout = new QHBoxLayout;
        rowlayout->addSpacing(5);
        rowlayout->addWidget(stateLabel);
        rowlayout->addStretch(1);
        rowlayout->addWidget(textLabel);
        rowlayout->addStretch(1);
        rowlayout->addWidget(bagButton);
        rowlayout->addSpacing(5);
        listLayout->addLayout(rowlayout);
    }

    // 获得离线用户
    sendInfo = {
        {"type", GET_OFFLINE_LIST}
    };
    receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    if (receiveInfo["type"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取用户列表失败"));
        return;
    }

    for(const auto& item : receiveInfo["info"])
    {
        std::string str = item.get<std::string>();

        // 显示用户状态
        QLabel *stateLabel = new QLabel();
        stateLabel->setPixmap(QPixmap(":/offline")
                              .scaled(36, 36, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        // 显示用户名
        QLabel *textLabel = new QLabel();
        textLabel->setText(QString::fromStdString(str));
        textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");

        QPushButton *bagButton = new QPushButton();
//        bagButton->setFlat(true);
//        bagButton->resize(48, 48);
//        bagButton->setStyleSheet("image: url(:/ball)");
        _signalMapper->setMapping(bagButton, QString::fromStdString(str));
        connect(bagButton, SIGNAL(clicked()), _signalMapper, SLOT(map()));


        // 构建显示条目的layout
        QHBoxLayout *rowlayout = new QHBoxLayout;
        rowlayout->addSpacing(5);
        rowlayout->addWidget(stateLabel);
        rowlayout->addStretch(1);
        rowlayout->addWidget(textLabel);
        rowlayout->addStretch(1);
        rowlayout->addWidget(bagButton);
        rowlayout->addSpacing(5);
        listLayout->addLayout(rowlayout);
    }

    ui->userListArea->setWidget(containWidget);
    connect(_signalMapper, SIGNAL(mapped(QString)), this, SLOT(ShowBag(QString)));
}

void UserListWidget::ShowBag(QString username)
{
    json sendInfo = {
        {"type", GET_USER_BAG},
        {"username", username.toStdString()}
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
    containWidget->setStyleSheet("background-color: rgb(255, 175, 77);");

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

void UserListWidget::Back()
{
    emit back();
}
