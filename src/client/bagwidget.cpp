#include "bagwidget.h"
#include "ui_bagwidget.h"

#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>

#include "../define.h"
#include "include/json.hpp"
using json = nlohmann::json;

BagWidget::BagWidget(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent),
    _client(client),
    ui(new Ui::BagWidget)
{
    ui->setupUi(this);
    InitUi();
    InitConnect();
}

void BagWidget::InitUi()
{
    // 设置界面大小
    setFixedSize(1280, 720);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/bag");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    // 设置返回按钮大小
    ui->returnButton->resize(48, 48);
}

void BagWidget::InitConnect()
{
    // 返回按钮点击触发back信号，使得退回主界面
    connect(ui->returnButton, &QPushButton::clicked, [this](){ emit back();});
}


void BagWidget::SetBag()
{
    // 在GUI上设置用户信息
    // @param:
    //      username 用户名
    //      rate 用户胜率
    auto setInfo = [this](std::string username, double rate) {
        // 构建获得用户成就请求信息
        json sendInfo = {
            {"define", GET_USER_ACH},
            {"username", username}
        };
        json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

        if (receiveInfo["define"].get<int>() == SERVER_ERROR)
        {
            QMessageBox::information(this, "Error", QString::fromLocal8Bit("获取用户信息失败"));
            return;
        }

        // 设置用户名显示
        auto str = "用户名: " + username;
        ui->usernameLabel->setText(QString::fromLocal8Bit(str.c_str()));

        // 设置胜率显示
        std::stringstream rateStream;
        rateStream << std::setiosflags(std::ios::fixed) << std::setprecision(1) << rate;
        str = "胜率: "+ rateStream.str() + "%";
        ui->rateLabel->setText(QString::fromLocal8Bit(str.c_str()));

        // 获得相应勋章的资源文件名
        // @param:
        //      相应成就的数量
        auto getFileName = [](int num) {
            if (num < 5)
                return ":/bronze";
            else if (num < 10)
                return ":/silver";
            else
                return ":/golden";
        };

        // 设置成就勋章显示
        ui->sumLabel->setPixmap(
                    QPixmap(getFileName(receiveInfo["sum_ach"].get<int>()))
                .scaled(20, 24, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->sumLabel->setToolTip(QString::fromStdString(
                                     std::to_string(receiveInfo["sum_ach"].get<int>())));
        ui->adsumLabel->setPixmap(
                    QPixmap(getFileName(receiveInfo["advance_ach"].get<int>()))
                .scaled(20, 24, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->adsumLabel->setToolTip(QString::fromStdString(
                                       std::to_string(receiveInfo["advance_ach"].get<int>())));
    };

    // 构建查看用户背包请求信息
    auto username = _client->GetUserName();
    json sendInfo = {
        {"define", GET_USER_BAG},
        {"username", username}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    if (receiveInfo["define"].get<int>() == SERVER_ERROR)
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

    // 将背包里的每个小精灵加载到gridlayout中
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

    // 用空layout填充第一行的空位置
    if (row == 1 && col != 1)
        for (int i = col; i <= 3; i++)
        {
            QVBoxLayout *rowlayout = new QVBoxLayout();
            QLabel *picLabel = new QLabel();
            picLabel->resize(120, 120);
            rowlayout->addWidget(picLabel);
            gridLayout->addLayout(rowlayout, 1, i);
        }

    gridLayout->setAlignment(Qt::AlignTop);
    ui->bagListArea->setWidget(containWidget);

}

BagWidget::~BagWidget()
{
    delete ui;
}
