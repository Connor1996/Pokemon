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
    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/background");

    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);
}

void Widget::InitConnect()
{
    // 触发登陆
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(Login()));
    // 触发注册
    connect(ui->signupButtion, SIGNAL(clicked()), this, SLOT(Signup()));
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
        // 发送登陆请求
       _client = new Client(username);
       json sendInfo = {
           {"define", LOG_IN},
           {"username", username},
           {"password", password}
       };
       json receiveInfo = json::parse(_client->Connect(sendInfo.dump()));

       // 检查是否登陆成功
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
           // 进入主界面
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
        // 发送注册请求
        json sendInfo = {
            {"define", SIGN_UP},
            {"username", username},
            {"password", password}
        };
        json receiveInfo = json::parse(tempConnection.Connect(sendInfo.dump()));

        // 检查是否注册成功
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
