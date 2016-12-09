#include "bagwidget.h"
#include "ui_bagwidget.h"

BagWidget::BagWidget(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent), _client(client),
    ui(new Ui::BagWidget)
{
    ui->setupUi(this);
    InitUi();
    InitConnect();
}

void BagWidget::InitUi()
{
    setFixedSize(1280, 720);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/bag");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    ui->returnButton->resize(48, 48);
}

void BagWidget::InitConnect()
{
    connect(ui->returnButton, SIGNAL(clicked()), this, SLOT(Back()));
}

void BagWidget::Back()
{
    emit back();
}

BagWidget::~BagWidget()
{
    delete ui;
}
