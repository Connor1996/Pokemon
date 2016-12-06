﻿#include "stackwidget.h"
#include "ui_stackwidget.h"

StackWidget::StackWidget(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent), _client(client),
    ui(new Ui::StackWidget),
    _gameLobby(new GameLobby(client)),
    _userListWidget(new UserListWidget(client))
{
    ui->setupUi(this);

    setFixedSize(1280, 720);
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置关闭按钮
    _stackLayout = new QStackedLayout(this);
    _stackLayout->addWidget(_gameLobby);
    _stackLayout->addWidget(_userListWidget);
    _stackLayout->setCurrentIndex(0);

    connect(_gameLobby, SIGNAL(clicked(int)), this, SLOT(SetCurrentIndex(int)));
    connect(_userListWidget, SIGNAL(back()), this, SLOT(BackToLobby()));
}

StackWidget::~StackWidget()
{
    delete ui;
}

void StackWidget::SetCurrentIndex(int index)
{
    _stackLayout->setCurrentIndex(index);
}

void StackWidget::BackToLobby()
{
    _stackLayout->setCurrentIndex(0);
}
