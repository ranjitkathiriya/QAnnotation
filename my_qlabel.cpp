#include "my_qlabel.h"
#include <stdio.h>


my_qlabel::my_qlabel(QWidget *parent) :
    QLabel(parent)
{
    mousePressed = QPoint(0,0);
    mouseReleased = QPoint(0,0);
    rectWidth = 0;
    rectHeight = 0;
    mPressed = false;
}

void my_qlabel::mouseMoveEvent(QMouseEvent *ev)
{
    if (mPressed) {
        mouseReleased = ev->pos();
        if (mouseReleased != mousePressed) {
            rectWidth = mouseReleased.x() - mousePressed.x();
            rectHeight = mouseReleased.y() - mousePressed.y();
            emit Mouse_Pos();
        }
    }
}

void my_qlabel::mousePressEvent(QMouseEvent *ev)
{
    mPressed = true;
    this->x = ev->pos().x();
    this->y = ev->pos().y();
    mousePressed = ev->pos();
    emit Mouse_Pressed();
}


void my_qlabel::mouseReleaseEvent(QMouseEvent *ev)
{
    mouseReleased = ev->pos();
    mPressed = false;
    if (mouseReleased != mousePressed) {

        rectWidth = mouseReleased.x() - mousePressed.x();
        rectHeight = mouseReleased.y() - mousePressed.y();
        emit Mouse_Release();
    }
}



