#include "scratchwidget.h"
#include "ui_scratchwidget.h"

ScratchWidget::ScratchWidget(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent), _client(client),
    ui(new Ui::ScratchWidget)
{
    ui->setupUi(this);
    InitUi();
    InitConnect();
}

void ScratchWidget::InitUi()
{
    setFixedSize(1280, 720);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/scratch");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    ui->returnButton->resize(48, 48);
}

void ScratchWidget::InitConnect()
{
    connect(ui->returnButton, SIGNAL(clicked()), this, SLOT(Back()));
}

void ScratchWidget::Back()
{
    emit back();
}

ScratchWidget::~ScratchWidget()
{
    delete ui;
}
