#include "bagwidget.h"
#include "ui_bagwidget.h"

#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>

#include "../define.h"
#include "include/json.hpp"
using json = nlohmann::json;

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


void BagWidget::SetBag()
{
    auto setInfo = [this](std::string username, double rate) {
        json sendInfo = {
            {"type", GET_USER_ACH},
            {"username", username}
        };
        json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

        if (receiveInfo["type"].get<int>() == SERVER_ERROR)
        {
            QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取用户信息失败"));
            return;
        }

        auto str = "用户名: " + username;
        ui->usernameLabel->setText(QString::fromLocal8Bit(str.c_str()));

        std::stringstream rateStream;
        rateStream << std::setiosflags(std::ios::fixed) << std::setprecision(1) << rate;
        str = "胜率: "+ rateStream.str() + "%";
        ui->rateLabel->setText(QString::fromLocal8Bit(str.c_str()));

        auto getFileName = [](int num) {
            if (num < 20)
                return ":/bronze";
            else if (num < 50)
                return ":/silver";
            else
                return ":/golden";
        };

        ui->sumLabel->setPixmap(
                    QPixmap(getFileName(receiveInfo["sum_ach"].get<int>()))
                .scaled(20, 24, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->adsumLabel->setPixmap(
                    QPixmap(getFileName(receiveInfo["advance_ach"].get<int>()))
                .scaled(20, 24, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    };

    auto username = _client->GetUserName();
    json sendInfo = {
        {"type", GET_USER_BAG},
        {"username", username}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    if (receiveInfo["type"].get<int>() == SERVER_ERROR)
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取背包信息失败"));
        return;
    }

    // 设置用户信息
    setInfo(username, receiveInfo["rate"].get<double>());

    // 判断是否已经有widget，有则删除
    if(ui->bagListArea->widget() != 0)
        delete ui->bagListArea->widget();


    QWidget *containWidget = new QWidget(ui->bagListArea);
    containWidget->setStyleSheet("background-color: rgb(141, 220, 163);");

    QGridLayout *gridLayout = new QGridLayout(containWidget);
    containWidget->setLayout(gridLayout);

    auto row = 1, col = 1;
    for (const auto& item: receiveInfo["info"])
    {
        json itemInfo = json::parse(item.get<std::string>());

        // 显示精灵图片
        QLabel *picLabel = new QLabel();
        QPixmap pic(QString::fromStdString("://images/static/" +
                                            itemInfo["name"].get<std::string>() + ".png"));
        picLabel->setPixmap(pic.scaled(120, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        // 构建提示字符串
        std::string infoStr;
        for (json::iterator it = itemInfo.begin(); it != itemInfo.end(); ++it)
            infoStr += it.key() + " : " + it.value().get<std::string>() + "\n";
        infoStr.pop_back();
        picLabel->setToolTip(QString::fromStdString(infoStr));

        // 显示等级
        QLabel *textLabel = new QLabel();
        textLabel->setText(QString::fromStdString("Lv." + itemInfo["level"].get<std::string>()));
        textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");
        textLabel->setAlignment(Qt::AlignCenter);

        QVBoxLayout *rowlayout = new QVBoxLayout();
        rowlayout->addWidget(picLabel);
        rowlayout->addWidget(textLabel);
        gridLayout->addLayout(rowlayout, row, col);

        if (++col > 3)
        {
            row++;
            col = 1;
        }
    }
    gridLayout->setAlignment(Qt::AlignTop);
    ui->bagListArea->setWidget(containWidget);

}

void BagWidget::Back()
{
    emit back();
}

BagWidget::~BagWidget()
{
    delete ui;
}
