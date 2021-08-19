#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clickable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

    ui->label->setContentsMargins(0,0,0,0);

    ui->label->setAlignment (Qt::AlignCenter);

    ui->label->setMouseTracking(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void my_qlabel::mouseReleaseEvent(QMouseEvent *ev)
{
    emit Mouse_Release();
}


void MainWindow::Mouse_Current_Pos()
{

}

void MainWindow::Mouse_Pressed()
{
    point1 = QPoint(ui->label->x, ui->label->y);
    drag = 1;
}

void MainWindow::Mouse_Release()
{
    QPainter painter(&m_logo_pic);
    QPen Red((Qt::green),8);
    painter.setPen(Red);
    painter.setFont(QFont("times",40,QFont::ExtraBold));
    painter.drawRect(point1.x(),point1.y(),ui->label->x,ui->label->y);

    ui->label->setPixmap(m_logo_pic.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}


void MainWindow::viewListImageview(QString imagepath)
{
    m_logo_pic.load(path+"/"+imagepath);
    ui->label->setPixmap(m_logo_pic);
    ui->label->setPixmap(m_logo_pic.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ROS Service Here.

    QStringList query = imagepath.split(".");
    updatetxt(path+"/"+query[0]);
}

// List view Double click for opening image in lbl.
void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{

    viewListImageview(index.data().toString());
}

// Directory for image select
void MainWindow::on_actionSave_Label_Directory_triggered()
{
    path = QFileDialog::getExistingDirectory(this, tr("Choose catalog"), ".", QFileDialog::ReadOnly);
//    qDebug() << "Hello world!" << path;

    QStringListModel *model = new QStringListModel(this);
    QDir directory(path+"/");
    QStringList images = directory.entryList(QStringList() <<"*.png" << "*.jpg" << "*.JPG",QDir::Files);
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

void MainWindow::updatetxt(QString fullpath){
    QFile file(fullpath+".txt");
    QString line;
    int line_count=0;
    QStringList list;
//    model1 = new QStringListModel(this);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            line_count++;
            QStringList list1 = line.split(' ');

            int x = qRound((list1[1].toDouble() - list1[3].toDouble() / 2) * m_logo_pic.width());
            int y = qRound((list1[2].toDouble() - list1[4].toDouble() / 2) * m_logo_pic.height());
            int w = qRound((list1[1].toDouble() + list1[3].toDouble() / 2) * m_logo_pic.width());
            int h = qRound((list1[2].toDouble() + list1[4].toDouble() / 2) * m_logo_pic.height());

            QPainter painter(&m_logo_pic);
            QPen Red((Qt::black),8);
            painter.setPen(Red);


            painter.setFont(QFont("times",40,QFont::ExtraBold));

            painter.drawRect(x,y,w-x,h-y);
            painter.drawText(x, y-10, "Label "+ QString::number(line_count));
            ui->label->setPixmap(m_logo_pic.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            list<< "Label_"+ QString::number(line_count);
        }
        model1 = new QStringListModel(this);
        model1->setStringList(list);
        ui->listView_2->setModel(model1);
        file.close();
    }
    else{
        list<<"Null";
        model1 = new QStringListModel(this);
        model1->setStringList(list);
        ui->listView_2->setModel(model1);
        file.close();
    }
}

void MainWindow::on_listView_2_doubleClicked(const QModelIndex &index)
{
    count_list2 = index.data().toString().split("_")[1].toInt();
    QStringList pathLast = ui->listView->currentIndex().data().toString().split(".");
    QFile file(path+"/"+pathLast[0]+".txt");
    QString line;
    int line_count=1;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            if(index.data().toString().split("_")[1].toInt() == line_count){
                QStringList list1 = line.split(' ');
                int x = qRound((list1[1].toDouble() - list1[3].toDouble() / 2) * m_logo_pic.width());
                int y = qRound((list1[2].toDouble() - list1[4].toDouble() / 2) * m_logo_pic.height());
                int w = qRound((list1[1].toDouble() + list1[3].toDouble() / 2) * m_logo_pic.width());
                int h = qRound((list1[2].toDouble() + list1[4].toDouble() / 2) * m_logo_pic.height());
                QPainter painter(&m_logo_pic);
                QPen Red((Qt::green),8);
                painter.setPen(Red);
                painter.setFont(QFont("times",40,QFont::ExtraBold));
                painter.drawRect(x,y,w-x,h-y);
                painter.drawText(x, y-10, "Label "+ QString::number(line_count));
                ui->label->setPixmap(m_logo_pic.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }else{
                QStringList list1 = line.split(' ');
                int x = qRound((list1[1].toDouble() - list1[3].toDouble() / 2) * m_logo_pic.width());
                int y = qRound((list1[2].toDouble() - list1[4].toDouble() / 2) * m_logo_pic.height());
                int w = qRound((list1[1].toDouble() + list1[3].toDouble() / 2) * m_logo_pic.width());
                int h = qRound((list1[2].toDouble() + list1[4].toDouble() / 2) * m_logo_pic.height());
                QPainter painter(&m_logo_pic);
                QPen Red((Qt::black),8);
                painter.setPen(Red);
                painter.setFont(QFont("times",40,QFont::ExtraBold));
                painter.drawRect(x,y,w-x,h-y);
                painter.drawText(x, y-10, "Label "+ QString::number(line_count));
                ui->label->setPixmap(m_logo_pic.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            line_count++;
        }
    }
    file.close();
}

void MainWindow::on_actionOpen_triggered()
{
    QString selfilter = tr("JPEG (*.jpg *.jpeg)");
    QString fileName = QFileDialog::getOpenFileName(
            this,
            "Select File",
            "/home",
            tr("All files (*.*);;JPEG (*.jpg *.jpeg);;TIFF (*.tif)" ),
            &selfilter);

    QStringListModel *model = new QStringListModel(this);
    int pos = fileName.lastIndexOf(QChar('/'));
    qDebug() << fileName.split("/").takeLast();
    path = fileName.left(pos);
    QStringList list = model->stringList();
    list.append(fileName.split("/").takeLast());
    model->setStringList(list);
    ui->listView->setModel(model);
}

void MainWindow::on_actionReset_All_triggered()
{
    qDebug()<< ui->listView->currentIndex().data().toString().split(".")[0];

    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete labeling file?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
        QFile::remove(path+"/"+ui->listView->currentIndex().data().toString().split(".")[0]+".txt");

        QStringList list;
        list<<"Null";
        model1 = new QStringListModel(this);
        model1->setStringList(list);
        ui->listView_2->setModel(model1);
      }
      // Call images label rect function -> that need's to create.
}

void MainWindow::on_actionRemove_RectBox_triggered()
{
    qDebug()<< ui->listView->currentIndex().data().toString();
    qDebug()<<count_list2;
    int counter = 1;

    QFile file(path+"/"+ui->listView->currentIndex().data().toString().split(".")[0]+".txt");
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&file);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            qDebug()<< line;

            if(counter != count_list2)
                s.append(line + "\n");
            counter ++;
        }
        file.resize(0);
        t << s;
        file.close();
    }

    viewListImageview(ui->listView->currentIndex().data().toString());

    QString line;
    int line_count=0;
    QStringList list;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            line_count++;
            QStringList list1 = line.split(' ');

            int x = qRound((list1[1].toDouble() - list1[3].toDouble() / 2) * m_logo_pic.width());
            int y = qRound((list1[2].toDouble() - list1[4].toDouble() / 2) * m_logo_pic.height());
            int w = qRound((list1[1].toDouble() + list1[3].toDouble() / 2) * m_logo_pic.width());
            int h = qRound((list1[2].toDouble() + list1[4].toDouble() / 2) * m_logo_pic.height());

            QPainter painter(&m_logo_pic);
            QPen Red((Qt::black),8);
            painter.setPen(Red);

            painter.setFont(QFont("times",40,QFont::ExtraBold));

            painter.drawRect(x,y,w-x,h-y);
            painter.drawText(x, y-10, "Label "+ QString::number(line_count));
            ui->label->setPixmap(m_logo_pic.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            list<< "Label_"+ QString::number(line_count);
        }
        model1 = new QStringListModel(this);
        model1->setStringList(list);
        ui->listView_2->setModel(model1);
        file.close();
    }
    else{
        list<<"Null";
        model1 = new QStringListModel(this);
        model1->setStringList(list);
        ui->listView_2->setModel(model1);
        file.close();
    }
}

// Rectangle box create
void MainWindow::on_actionCreate_Rectbox_triggered()
{
    ui->label->setCursor(Qt::CrossCursor);

    connect(ui->label, SIGNAL(Mouse_Pos()), this, SLOT(Mouse_Current_Pos()));
    connect(ui->label, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));
    connect(ui->label, SIGNAL(Mouse_Release()), this, SLOT(Mouse_Release()));

    // Check mouse click events it is there in line 26-56
    // 1. Select image then Image will be display in left listview and then after selecting I should be able to create rectangle box with the same as image coordinate.
        // Just like this video i should be able to create bounding box(Rectangle) https://www.youtube.com/watch?v=p0nR2YsCY_U&ab_channel=TzuTaLin
    // 2. display that image coordinate here. that's it.

    // Saving function is ready with me I will make it.
}










