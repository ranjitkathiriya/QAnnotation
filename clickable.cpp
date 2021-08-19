#include "clickable.h"
#include <QDebug>

void Clickable::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
//    qDebug()<< "Hello World";

}
