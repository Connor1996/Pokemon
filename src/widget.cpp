#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>


#include "include/json.hpp"
#include "define.h"
using Connor_Socket::Client;
using json = nlohmann::json;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    InitConnect();
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
           {"type", LOG_IN},
           {"username", username},
           {"password", password}
       };
       json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

       if (receiveInfo["type"].get<int>() == LOG_IN_FAIL)
       {
           _client->Close();
           delete _client;
           QMessageBox::information(this, "Error", QString::fromWCharArray(L"登陆失败"));
       }
       else if (receiveInfo["type"].get<int>() == LOG_IN_SUCCESS)
       {
           QMessageBox::information(this, "Message", QString::fromWCharArray(L"登陆成功"));
       }
       else if (receiveInfo["type"].get<int>() == SERVER_ERROR)
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
        std::cout << "[ERROR] " << e.what() << std::endl;
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
            {"type", SIGN_UP},
            {"username", username},
            {"password", password}
        };
        json receiveInfo = json::parse(tempConnection.Send(sendInfo.dump()));

        if (receiveInfo["type"].get<int>() == SIGN_UP_FAIL)
        {
            tempConnection.Close();
            QMessageBox::information(this, "Error", QString::fromWCharArray(L"注册失败，用户名已被注册"));
        }
        else if (receiveInfo["type"].get<int>() == SIGN_UP_SUCCESS)
        {
            QMessageBox::information(this, "Message", QString::fromWCharArray(L"注册成功"));
        }
        else if (receiveInfo["type"].get<int>() == SERVER_ERROR)
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
         std::cout << "[ERROR] " << e.what() << std::endl;
    }
}
