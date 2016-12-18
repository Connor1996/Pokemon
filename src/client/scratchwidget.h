#ifndef SCRATCHWIDGET_H
#define SCRATCHWIDGET_H

#include <QWidget>
#include "client.h"

namespace Ui {
class ScratchWidget;
}

class ScratchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScratchWidget(Connor_Socket::Client *client, QWidget *parent = 0);
    ~ScratchWidget();

signals:
    void back();

public slots:
    void Back();

private:
    void InitUi();
    void InitConnect();

    Ui::ScratchWidget *ui;
    Connor_Socket::Client *_client;
};

#endif // SCRATCHWIDGET_H
