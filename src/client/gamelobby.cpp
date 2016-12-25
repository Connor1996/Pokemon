#include "gamelobby.h"
#include "ui_gamelobby.h"

#include <QMovie>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QMessageBox>
#include <QLabel>
#include <QStackedLayout>
#include <QHBoxLayout>

#include "userlistwidget.h"
#include "include/json.hpp"
#include "../define.h"

using json = nlohmann::json;

GameLobby::GameLobby(Connor_Socket::Client *client, QWidget *parent) :
    QWidget(parent),
    _client(client),
    ui(new Ui::GameLobby)
{
    ui->setupUi(this);
    InitUi();
    InitConnect();

    ui->closeButton->resize(48, 48);
}

GameLobby::~GameLobby()
{
    delete ui;
}


void GameLobby::mousePressEvent(QMouseEvent *event)
{
    // 鼠标坐标是否在多边形区域中
    auto isPointInPolygon = []( QVector<QPointF> points, QPointF point )
    {
        QPolygonF polygon(points);
        return polygon.containsPoint(point, Qt::OddEvenFill);
    };

    static const QVector<QPointF> pointsForUserList = { QPointF(0, 0), QPointF(54, 0),
                                                        QPointF(615, 719), QPointF(0, 719) };
    static const QVector<QPointF> pointsForScratch = { QPointF(54, 0), QPointF(672, 0),
                                                       QPointF(600, 697) };
    static const QVector<QPointF> pointsForBag = { QPointF(664, 82), QPointF(600, 697),
                                    QPointF(615, 719), QPointF(1279, 677), QPointF(1279, 719) };
    static const QVector<QPointF> pointsForFight = { QPointF(673, 0), QPointF(1279, 0),
                                                     QPointF(1279, 676), QPointF(665, 81) };
    auto x = event->x();
    auto y = event->y();

    // 判断鼠标所在区域，产生响应信号
    if (isPointInPolygon(pointsForUserList, std::move(QPointF(x, y))))
        emit clicked(1);
    else if (isPointInPolygon(pointsForScratch, std::move(QPointF(x, y))))
        emit clicked(2);
    else if (isPointInPolygon(pointsForBag, std::move(QPointF(x, y))))
        emit clicked(3);
    else if (isPointInPolygon(pointsForFight, std::move(QPointF(x, y))))
        emit clicked(4);
}

void GameLobby::InitUi()
{
    setFixedSize(1280, 720);
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/lobby");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

}

void GameLobby::InitConnect()
{
    // 关闭按钮点击触发登出信号
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(LogOut()));
}


void GameLobby::LogOut()
{
    // 释放所有资源
    this->close();
    emit closeAll();
}

