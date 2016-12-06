#include "userlistwidget.h"
#include "ui_userlistwidget.h"

UserListWidget::UserListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserListWidget)
{
    ui->setupUi(this);
    setFixedSize(1280, 720);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/userlist");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);
}

UserListWidget::~UserListWidget()
{
    delete ui;
}
