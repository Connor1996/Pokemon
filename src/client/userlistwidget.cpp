#include "userlistwidget.h"
#include "ui_userlistwidget.h"

#include <QLabel>
#include <QMessageBox>

#include "include/json.hpp"
#include "../define.h"
using json = nlohmann::json;

#include <iomanip>

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
    // 返回按钮点击触发back信号，使得退回主界面
    connect(ui->returnButton, &QPushButton::clicked, [this](){ emit back();});
}

void UserListWidget::SetUserList()
{
    // 获得在线用户列表
    json sendInfo = {
        {"define", GET_ONLINE_LIST}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取用户列表失败"));
        return;
    }

    // 判断是否已经有widget，有则删除
    if(ui->userListArea->widget() != 0)
        delete ui->userListArea->widget();


    QWidget *containWidget = new QWidget(ui->userListArea);
    containWidget->setStyleSheet("background-color: rgb(255, 175, 77);");

    QVBoxLayout *listLayout = new QVBoxLayout();
    listLayout->setAlignment(Qt::AlignTop);
    containWidget->setLayout(listLayout);

    _signalMapper = new QSignalMapper(containWidget);

    // 构建在线列表
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
        bagButton->setFlat(true);
        bagButton->setIcon(QPixmap(":ball"));
        // 注册到mapper
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
        {"define", GET_OFFLINE_LIST}
    };
    receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取用户列表失败"));
        return;
    }

    // 构建离线列表
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
        bagButton->setIcon(QPixmap(":ball"));
        bagButton->setFlat(true);

        // 注册到信号mapper
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
    // 向服务器发送获取用户背包内容请求
    json sendInfo = {
        {"define", GET_USER_BAG},
        {"username", username.toStdString()}
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
    containWidget->setStyleSheet("background-color: rgb(255, 175, 77);");

    QGridLayout *gridLayout = new QGridLayout(containWidget);
    containWidget->setLayout(gridLayout);
    gridLayout->setAlignment(Qt::AlignTop);

    // 构建用户背包显示的gridlayout
    auto row = 1, col = 1;
    for (const auto& item: receiveInfo["info"])
    {
        json itemInfo = json::parse(item.get<std::string>());

        // 显示精灵图片
        QLabel *picLabel = new QLabel();
        QPixmap pic(QString::fromStdString("://images/static/" +
                                            itemInfo["name"].get<std::string>() + ".png"));
        picLabel->setPixmap(pic.scaled(120, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        picLabel->setAlignment(Qt::AlignCenter);
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
        //rowlayout->setSizeConstraint(QLayout::SetFixedSize);
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

    ui->bagListArea->setWidget(containWidget);

    // 显示胜率
    std::stringstream rate;
    rate << std::setiosflags(std::ios::fixed) << std::setprecision(1)
         << receiveInfo["rate"].get<double>();
    auto str = "胜率: "+ rate.str() + "%";
    ui->rateLabel->setText(QString::fromLocal8Bit(str.c_str()));
}
