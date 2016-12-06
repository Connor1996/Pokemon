#ifndef STACKWIDGET_H
#define STACKWIDGET_H

#include <QWidget>
#include <QStackedLayout>

namespace Ui {
class StackWidget;
}

class StackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StackWidget(QWidget *parent = 0);
    ~StackWidget();

public slots:
    BackToLobby();

private:
    Ui::StackWidget *ui;
    QStackedLayout *_stackLayout;

};

#endif // STACKWIDGET_H
