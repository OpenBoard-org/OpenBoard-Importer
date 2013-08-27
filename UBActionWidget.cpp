#include "UBActionWidget.h"

UBActionWidget::UBActionWidget(QWidget *parent) :
    QWidget(parent)
{
}


void UBActionWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    emit actionTrig();
}
