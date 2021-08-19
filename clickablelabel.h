#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class ClickableLabel : public QLabel
{
Q_OBJECT
public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR);
    void mousePressEvent(QMouseEvent* event);


signals:
    void mouse_Pressed();


};

#endif // CLICKABLELABEL_H
