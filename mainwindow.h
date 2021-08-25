#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QFile>
#include <QMouseEvent>
#include <QtDebug>
#include <QDir>
#include <QStringListModel>
#include <QString>
#include <QFileDialog>
#include <QProcess>
#include <string>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QScrollArea>

#include <QPoint>
#include <QRect>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class lblMouse;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_actionSave_Label_Directory_triggered();
    void on_actionNext_triggered();
    void on_actionPrevious_triggered();
    void on_actionExit_triggered();
    void on_actionClose_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_2_triggered();
    void on_actionTutorial_triggered();
    void on_actionCreate_Rectbox_triggered();
    void on_listView_2_doubleClicked(const QModelIndex &index);
    void on_actionOpen_triggered();
    void on_actionRemove_RectBox_triggered();
    void on_actionReset_All_triggered();

    void Mouse_Current_Pos();
    void Mouse_Pressed();
    void Mouse_Release();


    void on_actionSave_RectBox_triggered();

    void on_actionAI_triggered();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QString path;
    QPixmap m_logo_pic;
    QStringListModel *model1;
    int count_list2;

    QPoint point1, point2;
    int drag = 0;
    QRect rect,rect_scale;
    int select_flag = 0;

    QPainter painter1;

    int image_width,image_height;
    QString result;

public:
    void viewListImageview(QString imagepath);
    void updatetxt(QString fullpath);

};
#endif // MAINWINDOW_H
