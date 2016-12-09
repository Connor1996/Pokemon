#ifndef BAGWIDGET_H
#define BAGWIDGET_H

#include <QWidget>
#include "client.h"

namespace Ui {
class BagWidget;
}

class BagWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BagWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~BagWidget();

signals:
    void back();

public slots:
    void Back();
    //void ShowBag(QString username);

private:
    void InitUi();
    void InitConnect();

    Ui::BagWidget *ui;
    Connor_Socket::Client *_client;
};

#endif // BAGWIDGET_H
