#ifndef UBACTIONWIDGET_H
#define UBACTIONWIDGET_H

#include <QWidget>

class UBActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UBActionWidget(QWidget *parent = 0);

signals:
    void actionTrig();

private:
    virtual void showEvent(QShowEvent *e);

};

#endif // UBACTIONWIDGET_H
