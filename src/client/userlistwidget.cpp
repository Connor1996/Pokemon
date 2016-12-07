#include "userlistwidget.h"
#include "ui_userlistwidget.h"

#include <QLabel>

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

void UserListWidget::SetOnlineList()
{
    json sendInfo = {
        {"type", GET_ONLINE_LIST}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    // 判断是否已经有layout，有则删除
    if(ui->userListWidget->layout() != 0)
        delete ui->userListWidget->layout();
    if(_signalMapper != nullptr)
    {
        _signalMapper->disconnect();
        delete _signalMapper;
    }
    _signalMapper = new QSignalMapper;

    QVBoxLayout *listLayout = new QVBoxLayout;
    for(const auto& item : receiveInfo)
    {
        std::string str = item.get<std::string>();

        // 显示用户状态
        QLabel *stateLabel = new QLabel();
        stateLabel->setPixmap(QPixmap(":/online"));
        // 显示用户名
        QLabel *textLabel = new QLabel();
        textLabel->setText(QString::fromStdString(str));
        //textLabel->setFont(QFont("微软雅黑"));

        QPushButton *bagButton = new QPushButton();
        bagButton->setFlat(true);
        //bagButton->resize(24, 24);
        bagButton->setStyleSheet("border-image: url(:/ball)");
        _signalMapper->setMapping(bagButton, QString::fromStdString(str));
        connect(bagButton, SIGNAL(clicked()), _signalMapper, SLOT(map()));


        // 构建显示条目的layout
        QHBoxLayout *rowlayout = new QHBoxLayout;
        rowlayout->addStretch(1);
        rowlayout->addWidget(stateLabel);
        rowlayout->addStretch(1);
        rowlayout->addWidget(textLabel);
        rowlayout->addStretch(1);
        rowlayout->addWidget(bagButton);
        rowlayout->addStretch(1);
        listLayout->addLayout(rowlayout);
    }
    listLayout->addStretch(1);
    ui->userListWidget->setLayout(listLayout);

    connect(_signalMapper, SIGNAL(mapped(QString)), this, SLOT(ShowBag(QString)));
}

void UserListWidget::ShowBag(QString username)
{
    json sendInfo = {
        {"type", GET_USER_BAG}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));
}

void UserListWidget::Back()
{
    emit back();
}
