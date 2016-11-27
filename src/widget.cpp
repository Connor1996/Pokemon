#include "widget.h"
#include "ui_widget.h"

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
       Socket::Client client = Socket::Client(username);

    } catch (std::exception e){
        std::cout << e.what() << std::endl;
    }
}
