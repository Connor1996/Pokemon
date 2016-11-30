#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QTextCodec>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    InitConnect();
}

void Widget::InitConnect()
{
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(Login()));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::Login()
{
    auto username = ui->usernameEdit->text().toStdString();
    auto password = ui->passwordEdit->text().toStdString();

    try
    {
       client = new Connor_Socket::Client(username);
       if (client->Connect(password))
       {
           QMessageBox::information(this, "Message", QString::fromWCharArray(L"登陆成功"));
       }
       else
           QMessageBox::information(this, "Error", QString::fromWCharArray(L"登陆失败"));
    } catch (std::exception e){
        delete client;
        std::cout << e.what() << std::endl;
    }
}
