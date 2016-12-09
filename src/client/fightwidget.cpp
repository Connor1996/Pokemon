#include "fightwidget.h"
#include "ui_fightwidget.h"

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

void FightWidget::Back()
{
    emit back();
}


FightWidget::~FightWidget()
{
    delete ui;
}
