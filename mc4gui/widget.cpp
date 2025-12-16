#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("MC4GUI");

    M.init(6);

    connect(ui->btGoPos, SIGNAL(clicked()), this, SLOT(onGoPos()));
    connect(ui->btReadPos, SIGNAL(clicked()), this, SLOT(onReadPos()));
    connect(ui->btReadStat, SIGNAL(clicked()), this, SLOT(onReadStat()));
    connect(ui->btReset, SIGNAL(clicked()), this, SLOT(onReset()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onGoPos()
{
    int pos = ui->sbSetPosition->value();
    char ax = getAxis();
    M.set_position(ax, pos);
}

void Widget::onReadPos()
{
    int pos;
    M.get_position(getAxis(),pos);
    ui->readPosition->setText(QString::number(pos));
}

void Widget::onReadStat()
{
    int st = 0;
    ui->readStatus->setText(QString::number(st));
}

void Widget::onReset()
{
    M.reset_position(getAxis());
}

char Widget::getAxis()
{
    const char* axes = "XYZW";
    int i = ui->cbAxes->currentIndex();
    return (i>=0 && i<4) ? axes[i] : 'Z';
}
