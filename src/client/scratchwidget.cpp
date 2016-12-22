#include "scratchwidget.h"
#include "ui_scratchwidget.h"

#include <QPropertyAnimation>
#include <QMessageBox>

#include "include/json.hpp"
#include "../define.h"
using json = nlohmann::json;

ScratchWidget::ScratchWidget(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent),
    _client(client),
    ui(new Ui::ScratchWidget)
{
    ui->setupUi(this);
    InitUi();
    InitConnect();

    // 初始化随机种子
    srand(time(NULL));
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
    ui->pokemonBall->installEventFilter(this);
}

void ScratchWidget::InitConnect()
{
    // 返回按钮点击触发back信号，使得退回主界面
    connect(ui->returnButton, &QPushButton::clicked, [this](){ emit back();});
    // 点击触发抽奖
    connect(ui->pokemonBall, &QPushButton::clicked, [=](){
        auto random = []()
        {
            //随机生成0-1的小数.
            return static_cast<double>(rand() % (1000 + 1)) / (1000 + 1);
        };

        if (random() < 0.01)
        {
            json sendInfo = {
                {"define", GET_ONE_POKEMON},
            };
            json receiveInfo = json::parse(_client->Send(sendInfo.dump()));
            auto str ="抓取成功，获得" + receiveInfo["name"].get<std::string>();
            QMessageBox::information(this, "INFO", QString::fromLocal8Bit(str.c_str()));
        }
        else
            QMessageBox::information(this, "INFO", QString::fromLocal8Bit("抓取失败"));
    });
}

bool ScratchWidget::eventFilter(QObject *watched, QEvent *event)
{
    // 对精灵球的监控
    if (watched == ui->pokemonBall)
    {
        // 如果鼠标移到精灵球上
        if (event->type() == QEvent::HoverEnter)
        {
            QPropertyAnimation *animation = new QPropertyAnimation(watched, "pos");

            auto dx = ui->pokemonBall->x();
            auto dy = ui->pokemonBall->y();

            // 设置震动动画
            animation->setDuration(200);
            animation->setLoopCount(3);
            animation->setKeyValueAt(0, QPoint(dx - 3, dy - 3));
            animation->setKeyValueAt(0.1, QPoint(dx + 6, dy + 6));
            animation->setKeyValueAt(0.2, QPoint(dx - 6, dy + 6));
            animation->setKeyValueAt(0.3, QPoint(dx + 6, dy - 6));
            animation->setKeyValueAt(0.4, QPoint(dx - 6, dy - 6));
            animation->setKeyValueAt(0.5, QPoint(dx + 6, dy + 6));
            animation->setKeyValueAt(0.6, QPoint(dx - 6, dy + 6));
            animation->setKeyValueAt(0.7, QPoint(dx + 6, dy - 6));
            animation->setKeyValueAt(0.8, QPoint(dx - 6, dy - 6));
            animation->setKeyValueAt(0.9, QPoint(dx + 6, dy + 6));
            animation->setKeyValueAt(1, QPoint(dx, dy));
            animation->start();
            return true;
        }

        return false;
    }
    return false;

}

ScratchWidget::~ScratchWidget()
{
    delete ui;
}
