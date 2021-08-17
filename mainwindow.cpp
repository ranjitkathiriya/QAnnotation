#include "mainwindow.h"
#include "ui_mainwindow.h"
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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->label->setContentsMargins(0,0,0,0);

    ui->label->setAlignment (Qt::AlignCenter);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::viewListImageview(QString imagepath)
{
    m_logo_pic.load(path+"/"+imagepath);
    ui->label->setPixmap(m_logo_pic);
    ui->label->setPixmap(m_logo_pic.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ROS Service Here.
}

// List view Double click for opening image in lbl.
void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    viewListImageview(index.data().toString());
//    m_logo_pic.load(path+"/"+index.data().toString());
//    ui->label->setPixmap(m_logo_pic);
//    ui->label->setPixmap(m_logo_pic.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// Directory for image select
void MainWindow::on_actionSave_Label_Directory_triggered()
{
    path = QFileDialog::getExistingDirectory(this, tr("Choose catalog"), ".", QFileDialog::ReadOnly);
//    qDebug() << "Hello world!" << path;

    QStringListModel *model = new QStringListModel(this);
    QDir directory(path+"/");
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG",QDir::Files);
    model->setStringList(images);
    ui->listView->setModel(model);
}

// Next Image
void MainWindow::on_actionNext_triggered()
{
    QModelIndex mi = ui->listView->currentIndex();

    if(mi.row() < ui->listView->model()->rowCount() - 1) {
        ui->listView->setCurrentIndex(mi.sibling(mi.row()+1,mi.column()));
        viewListImageview(ui->listView->currentIndex().data().toString());
    }
}

//Previous Image
void MainWindow::on_actionPrevious_triggered()
{
    QModelIndex mi = ui->listView->currentIndex();

    if(mi.row() > 0) {
        ui->listView->setCurrentIndex(mi.sibling(mi.row()-1,mi.column()));
        viewListImageview(ui->listView->currentIndex().data().toString());
    }
}

// Quit Application
void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

// Clear all content on screen trigger
void MainWindow::on_actionClose_triggered()
{
    qDebug() << "Closes";
    if(ui->listView->currentIndex().row() > 0) {
        ui->listView->model()->removeRows(0,ui->listView->model()->rowCount());
    //    ui->listView_2->model()->removeRows(0,ui->listView_2->model()->rowCount());
        ui->label->clear();
    }
}

// About Qt
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::aboutQt(this);
}

// About software
void MainWindow::on_actionAbout_2_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("This application is developed by DairyRobotic. \n\n Automatic annotation for cow teat.\n\n Author: Ranjit Kathiriya\n Email: ranjitkathiriya@gmail.com");
    msgBox.exec();
}

// Action tutorials
void MainWindow::on_actionTutorial_triggered()
{
    QPainter painter(&m_logo_pic);
    QPen Red((Qt::green),10);
    painter.setPen(Red);
    painter.drawLine(10,10,50,50);
    ui->label->setPixmap(m_logo_pic);

    // For fixing GUI image issue
    const int w = ui->label->width();
    const int h = ui->label->height();
    ui->label->setPixmap(m_logo_pic.scaled(w,h,Qt::KeepAspectRatio));
}

// Rectangle box create
void MainWindow::on_actionCreate_Rectbox_triggered()
{
    // [((985, 94), (1308, 674)), ((1160, 550), (1412, 1077)), ((1737, 539), (2059, 1057)), ((1753, 37), (2109, 637))]

//    QPainter painter(&m_logo_pic);
//    QPen Red((Qt::green),5);
//    painter.setPen(Red);
//    painter.drawRect(985,94,1308-985,674-94);

//    ui->label->setPixmap(m_logo_pic.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    qDebug() << path <<ui->listView->currentIndex().data().toString();
    QStringList query = ui->listView->currentIndex().data().toString().split(".");
    qDebug() << query[0];
    createboundingBox(path+"/"+query[0]);
}

void MainWindow::createboundingBox(QString fullpath){
    QFile file(fullpath+".txt");
    QString line;
    int line_count=0;

    qDebug()<< m_logo_pic.width() << m_logo_pic.height();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            line_count++;
            QStringList list1 = line.split(' ');

            qDebug()<< list1[0] << QString::number(line_count);

            int x = qRound((list1[1].toDouble() - list1[3].toDouble() / 2) * m_logo_pic.width());
            int y = qRound((list1[2].toDouble() - list1[4].toDouble() / 2) * m_logo_pic.height());
            int w = qRound((list1[1].toDouble() + list1[3].toDouble() / 2) * m_logo_pic.width());
            int h = qRound((list1[2].toDouble() + list1[4].toDouble() / 2) * m_logo_pic.height());

            qDebug()<<x <<y <<w <<h;

            QPainter painter(&m_logo_pic);
            QPen Red((Qt::green),8);
            painter.setPen(Red);
            painter.drawRect(x,y,w-x,h-y);
            ui->label->setPixmap(m_logo_pic.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    file.close();
}




