#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "mc4.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected slots:

    void onGoPos();
    void onReadPos();
    void onReadStat();
    void onReset();

private:
    mc4 M;
    Ui::Widget *ui;

    char getAxis();
};
#endif // WIDGET_H
