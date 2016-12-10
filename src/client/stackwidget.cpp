#include "stackwidget.h"
#include "ui_stackwidget.h"
#include <QGraphicsDropShadowEffect>

StackWidget::StackWidget(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent), _client(client),
    ui(new Ui::StackWidget),
    _gameLobby(new GameLobby(client)),
    _userListWidget(new UserListWidget(client)),
    _bagWidget(new BagWidget(client)),
    _fightWidget(new FightWidget(client))
{
    ui->setupUi(this);
    InitUi();
    InitConnect();
}

void StackWidget::InitUi()
{
    setFixedSize(1280, 720);
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置关闭按钮
    _stackLayout = new QStackedLayout(this);
    _stackLayout->addWidget(_gameLobby);
    _stackLayout->addWidget(_userListWidget);
    _stackLayout->addWidget(new UserListWidget(_client));
    _stackLayout->addWidget(_bagWidget);
    _stackLayout->addWidget(_fightWidget);
    _stackLayout->setCurrentIndex(0);
}

void StackWidget::InitConnect()
{
    connect(_gameLobby, SIGNAL(clicked(int)), this, SLOT(SetCurrentIndex(int)));
    connect(_userListWidget, SIGNAL(back()), this, SLOT(BackToLobby()));
    connect(_bagWidget, SIGNAL(back()), this, SLOT(BackToLobby()));
    connect(_fightWidget, SIGNAL(back()), this, SLOT(BackToLobby()));

    // 点击的时候刷新相应界面内容
    connect(this, SIGNAL(refreshUserList()), _userListWidget, SLOT(SetUserList()));
    connect(this, SIGNAL(refreshBag()), _bagWidget, SLOT(SetBag()));

    connect(_gameLobby, SIGNAL(closeAll()), this, SLOT(close()));
}

StackWidget::~StackWidget()
{
    delete ui;
    delete _gameLobby;
    delete _userListWidget;
    delete _bagWidget;
    delete _fightWidget;
    delete _client;
}

void StackWidget::SetCurrentIndex(int index)
{
    if (index == 1)
        emit refreshUserList();
    else if (index == 3)
        emit refreshBag();
    _stackLayout->setCurrentIndex(index);

}

void StackWidget::BackToLobby()
{
    _stackLayout->setCurrentIndex(0);
}
