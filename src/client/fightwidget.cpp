#include "fightwidget.h"
#include "fightroom.h"
#include "ui_fightwidget.h"

#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>

#include "../pokemonfactory.h"
#include "../define.h"
#include "../model/pokemonlist.h"
#include "include/json.hpp"
using json = nlohmann::json;

FightWidget::FightWidget(Connor_Socket::Client *client, QWidget *parent) :
    _parent(parent), _client(client), _select(nullptr),
    ui(new Ui::FightWidget)
{
    ui->setupUi(this);
    InitUi();
    InitConnect();
}

void FightWidget::InitUi()
{
    setFixedSize(1280, 720);

    // 设置背景
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/fight");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    ui->returnButton->resize(48, 48);

    // 获取对战精灵列表
    json sendInfo = {
        {"type", GET_POKEMON_LIST}
    };
    json receiveInfo = json::parse(_client->Send(sendInfo.dump()));

    if (receiveInfo["type"].get<int>() == QUERY_SUCCESS)
    {
        for(const auto& item : receiveInfo["info"])
        {
            QListWidgetItem *listItem = new QListWidgetItem(ui->listWidget);
            listItem->setText(QString::fromStdString(item.get<std::string>()));
            listItem->setTextAlignment(Qt::AlignHCenter);
            ui->listWidget->addItem(listItem);
        }
    }
    else
    {
        QMessageBox::information(this, "Error", QString::fromLocal8Bit("获得对战精灵列表失败"));
    }


}

void FightWidget::InitConnect()
{
    connect(ui->returnButton, SIGNAL(clicked()), this, SLOT(Back()));
    connect(ui->fightButton, SIGNAL(clicked()), this, SLOT(FightBegin()));
}

bool FightWidget::eventFilter(QObject *watched, QEvent *event)
{ 
    if (watched->inherits("QLabel"))
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            if (_select == nullptr)
            {
                _select = watched;
                reinterpret_cast<QLabel*>(watched)->setStyleSheet("background: rgba(0,0,0, 20%);"
                                                              "border-radius: 5px");
            }
            else
            {
                if (watched == _select)
                {
                    _select = nullptr;
                    reinterpret_cast<QLabel*>(watched)->setStyleSheet("");
                }
                else
                {
                    reinterpret_cast<QLabel*>(_select)->setStyleSheet("");
                    _select = watched;
                    reinterpret_cast<QLabel*>(watched)->setStyleSheet("background: rgba(0,0,0, 20%);"
                                                                  "border-radius: 5px");
                }
            }
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void FightWidget::SetBag()
{
    // 初始化_select
    _select = nullptr;
    removeEventFilter(this);

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

    // 判断是否已经有widget，有则删除
    if(ui->bagListArea->widget() != 0)
        delete ui->bagListArea->widget();


    QWidget *containWidget = new QWidget(ui->bagListArea);
    containWidget->setStyleSheet("background-color: rgb(255, 233, 140);");

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
        picLabel->installEventFilter(this);

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

void FightWidget::FightBegin()
{
    if (_select == nullptr || ui->listWidget->currentItem() == nullptr)
    {
        QMessageBox::information(this, "ERROR", QString::fromLocal8Bit("请选择对战的小精灵"));
        return;
    }

    Pokemon *fighter = PokemonFactory::CreateUser(
                reinterpret_cast<QLabel *>(_select)->toolTip().toStdString());
    Pokemon *againster = PokemonFactory::CreateComputer(
                ui->listWidget->currentItem()->text().toStdString(), _client);

    FightRoom *fightRoom = new FightRoom(fighter, againster);
    this->hide();//this->_parent->hide();
    fightRoom->show();
    this->show();
}

void FightWidget::Back()
{
    emit back();
}

FightWidget::~FightWidget()
{
    delete ui;
}
