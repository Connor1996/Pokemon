#include "userlistwidget.h"
#include "ui_userlistwidget.h"

#include "include/json.hpp"
#include "../define.h"
using json = nlohmann::json;

UserListWidget::UserListWidget(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent), _client(client),
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
    SetOnlineList();
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

    for(const auto& item : receiveInfo)
    {
        std::string str = item.get<std::string>();
        ui->onlineListWidget->addItem(QString::fromStdString(str));
    }
}

void UserListWidget::Back()
{
    emit back();
}
