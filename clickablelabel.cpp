#include "clickablelabel.h"



void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit mouse_Pressed();
}
