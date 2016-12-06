#ifndef USERLISTWIDGET_H
#define USERLISTWIDGET_H

#include <QWidget>

namespace Ui {
class UserListWidget;
}

class UserListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserListWidget(QWidget *parent = 0);
    ~UserListWidget();

private:
    Ui::UserListWidget *ui;
};

#endif // USERLISTWIDGET_H
