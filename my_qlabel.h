#ifndef MY_QLABEL_H
#define MY_QLABEL_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

class my_qlabel : public QLabel
{
    Q_OBJECT
public:
    explicit my_qlabel(QWidget *parent = 0);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

    int x,y;
    int rectWidth, rectHeight;
    bool mPressed;
signals:
    void Mouse_Pressed();
    void Mouse_Pos();
    void Mouse_Release();


public slots:

private:
    QPoint mousePressed;
    QPoint mouseReleased;
};

#endif // MY_QLABEL_H
