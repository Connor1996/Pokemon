#ifndef FIGHTROOM_H
#define FIGHTROOM_H

#include <QWidget>
#include <QSignalMapper>
#include <QLabel>

#include "../pokemon.h"
#include "client.h"


using Client = Connor_Socket::Client;

namespace Ui {
class FightRoom;
}

// 实时对战界面
class FightRoom : public QWidget
{
    Q_OBJECT

public:
    explicit FightRoom(Pokemon *fighter, Pokemon *againster, Client *client,
                       bool isLose, QWidget *parent = 0);
    ~FightRoom();
    // 开始对战
    void Fight();
    // 重载关闭事件
    void FightRoom::closeEvent(QCloseEvent *event);

signals:
    // 小精灵普通攻击信号
    void attack(QLabel *, QLabel *);
    // 小精灵特殊攻击信号
    void attack_SP(std::pair<Pokemon *, QLabel *> *, std::pair<Pokemon *, QLabel *> *);
    // 游戏结束信号
    void over(Pokemon *);
    // 小精灵受伤害信号
    void hurt(QLabel *attacker, QLabel *suffer);
    // 窗口关闭信号
    void isClosed();
    // 清除掉血文字信号
    void clearText();
    // 隐藏招式标签信号
    void hideLabel();

private slots:
    // 设置普通攻击动画
    // @param:
    //      attacker GUI中攻击方的Label指针
    //      suffer GUI中被攻击方的Label指针
    void setAnimation(QLabel *attacker, QLabel *suffer);

    // 设置特殊攻击动画
    // @param:
    //      attacker 攻击方的小精灵对象指针和GUI中的显示Label
    //      suffer  被攻击方的小精灵对象指针和GUI中的显示Label
    void setAnimation_SP(std::pair<Pokemon *, QLabel *> *attacker,
                         std::pair<Pokemon *, QLabel *> *suffer);
    // 对战完成
    // @param:
    //      winner 胜利小精灵的对象指针
    void GameComplete(Pokemon *winner);

    // 更新双方血量
    // @param:
    //      attacker GUI中攻击方的Label指针
    //      suffer GUI中被攻击方的Label指针
    void UpdateHp(QLabel *attacker, QLabel *suffer);

    // 设置掉血数值
    void setText();

    // 设置招式Label
    void setLabel();

    // 选择损失的小精灵
    void Choose(int);

private:
    // 初始化UI界面
    void InitUi();

    // 初始化信号槽
    void InitConnect();

    // 该widget的ui界面指针
    Ui::FightRoom *ui;
    // 与服务器连接的socket指针
    Connor_Socket::Client *_client;
    // 对战用户方的小精灵对象指针和GUI中的显示Label
    std::pair<Pokemon *, QLabel *> _fighter;
    // 对战电脑方的小精灵对象指针和GUI中的显示Label
    std::pair<Pokemon *, QLabel *> _againster;
    // 该场对战用户失败是否会损失小精灵
    bool _isLose;
    // 在选择损失小精灵所用的信号mapper
    QSignalMapper *_signalMapper;
    // 标识用户在对战中是否中途退出
    bool _quit;
};

#endif // FIGHTROOM_H
