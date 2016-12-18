#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QPainter>
#include <QBrush>

#include "stackwidget.h"
#include "include/json.hpp"
#include "../define.h"

using Connor_Socket::Client;
using json = nlohmann::json;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    InitConnect();
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowCloseButtonHint); //只要关闭按钮

    setFixedSize(1024, 605);

    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/background");

    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);
}

void Widget::paintEvent(QPaintEvent *)
{
//    QPainter painter(this);
//    QBrush brush;
//    brush.setTextureImage(QImage(":background")); //背景图片
//    painter.setBrush(brush);
//    painter.setPen(Qt::black);  //边框色
//    painter.drawRoundedRect(this->rect(), 5, 5); //圆角5像素
}

void Widget::InitConnect()
{
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(Login()));
    connect(ui->signupButtion, SIGNAL(clicked()), this, SLOT(Signup()));
}

Widget::~Widget()
{
    delete ui;
//    if (_client != NULL)
//        delete _client;
}


void Widget::Login()
{
    auto username = ui->usernameEdit->text().toStdString();
    auto password = ui->passwordEdit->text().toStdString();

    try
    {
       _client = new Client(username);
       json sendInfo = {
           {"define", LOG_IN},
           {"username", username},
           {"password", password}
       };
       json receiveInfo = json::parse(_client->Connect(sendInfo.dump()));

       if (receiveInfo["define"].get<int>() == LOG_IN_FAIL_WP)
       {
           _client->Close();
           delete _client;
           QMessageBox::information(this, "Error", QString::fromLocal8Bit("登陆失败,用户名或密码错误"));
       }
       else if (receiveInfo["define"].get<int>() == LOG_IN_FAIL_AO)
       {
           _client->Close();
           delete _client;
           QMessageBox::information(this, "Error", QString::fromLocal8Bit("登陆失败，该用户已经在线"));
       }
       else if (receiveInfo["define"].get<int>() == LOG_IN_SUCCESS)
       {
           this->close();
           StackWidget *stack = new StackWidget(_client);
           try
           {
                stack->show();
           }
           catch (std::exception e)
           {
               QMessageBox::information(this, "Error", QString::fromLocal8Bit("与服务器断开连接"));
           }

           //lobby->exec();
           //this->show();
       }
       else if (receiveInfo["define"].get<int>() == SERVER_ERROR)
       {
           throw std::runtime_error("Server occurs fatal error");
       }
       else
       {
           throw std::runtime_error("Wrong return value for request");
       }
    } catch (std::exception e){
        _client->Close();
        delete _client;
        QMessageBox::information(this, "Error", QString(e.what()));
    }
}


void Widget::Signup()
{
    auto username = ui->usernameEdit->text().toStdString();
    auto password = ui->passwordEdit->text().toStdString();

    Client tempConnection;
    try
    {
        json sendInfo = {
            {"define", SIGN_UP},
            {"username", username},
            {"password", password}
        };
        json receiveInfo = json::parse(tempConnection.Connect(sendInfo.dump()));

        if (receiveInfo["define"].get<int>() == SIGN_UP_FAIL)
        {
            tempConnection.Close();
            QMessageBox::information(this, "Error", QString::fromLocal8Bit("注册失败，用户名已被注册"));
        }
        else if (receiveInfo["define"].get<int>() == SIGN_UP_SUCCESS)
        {
            QMessageBox::information(this, "Message", QString::fromLocal8Bit("注册成功"));
        }
        else if (receiveInfo["define"].get<int>() == SERVER_ERROR)
        {
            throw std::runtime_error("Server occurs fatal error");
        }
        else
        {
            throw std::runtime_error("Wrong return value for request");
        }
    }
    catch (std::exception e)
    {
         tempConnection.Close();
         QMessageBox::information(this, "Error", QString(e.what()));
    }
}
