#include "stackwidget.h"
#include "ui_stackwidget.h"

StackWidget::StackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StackWidget)
{
    ui->setupUi(this);

    setFixedSize(1280, 720);
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置关闭按钮
    _stackLayout = new QStackedLayout(this);
    _stackLayout->addWidget(new GameLobby());
    _stackLayout->addWidget(new UserListWidget());
    _stackLayout->setCurrentIndex(0);
}

StackWidget::~StackWidget()
{
    delete ui;
}
