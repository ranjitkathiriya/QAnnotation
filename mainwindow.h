#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
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

private:
    Ui::MainWindow *ui;
    QString path;
    QPixmap m_logo_pic;

public:
    void viewListImageview(QString imagepath);
    void createboundingBox(QString fullpath);

};
#endif // MAINWINDOW_H
